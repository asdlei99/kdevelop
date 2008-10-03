/* KDevelop CMake Support
 *
 * Copyright 2006 Matt Rogers <mattr@kde.org>
 * Copyright 2007-2008 Aleix Pol <aleixpol@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "cmakemanager.h"

#include <QList>
#include <QVector>
#include <QDomDocument>
#include <QDir>
#include <QQueue>

#include <KUrl>
#include <KProcess>
#include <KAction>
#include <KMessageBox>
#include <kio/job.h>

#include <interfaces/icore.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iproject.h>
#include <interfaces/iplugincontroller.h>
#include <interfaces/ilanguagecontroller.h>
#include <interfaces/contextmenuextension.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kaboutdata.h>
#include <project/projectmodel.h>
#include <language/duchain/parsingenvironment.h>
#include <language/duchain/indexedstring.h>
#include <interfaces/context.h>

#include <language/duchain/duchain.h>
#include <language/duchain/dumpchain.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/declaration.h>
#include <language/duchain/duchainlock.h>
#include <language/codecompletion/codecompletion.h>

#include "cmakeconfig.h"
#include "cmakemodelitems.h"
#include "cmakehighlighting.h"

#include "cmakecachereader.h"
#include "cmakeastvisitor.h"
#include "cmakeprojectvisitor.h"
#include "cmakeexport.h"
#include "icmakebuilder.h"
#include "cmakecodecompletionmodel.h"

#ifdef CMAKEDEBUGVISITOR
#include "cmakedebugvisitor.h"
#endif

using namespace KDevelop;

K_PLUGIN_FACTORY(CMakeSupportFactory, registerPlugin<CMakeProjectManager>(); )
K_EXPORT_PLUGIN(CMakeSupportFactory(KAboutData("kdevcmakemanager","kdevcmakemanager", ki18n("CMake Manager"), "0.1", ki18n("Support for managing CMake projects"), KAboutData::License_GPL)))

QString executeProcess(const QString& execName, const QStringList& args=QStringList())
{
    kDebug(9042) << "Executing:" << execName << "::" << args /*<< "into" << *m_vars*/;

    KProcess p;
    p.setOutputChannelMode(KProcess::MergedChannels);
    p.setProgram(execName, args);
    p.start();

    if(!p.waitForFinished())
    {
        kDebug() << "failed to execute:" << execName;
    }

    QByteArray b = p.readAllStandardOutput();
    QString t;
    t.prepend(b.trimmed());
    kDebug(9042) << "executed" << execName << "<" << t;

    return t;
}

CMakeProjectManager::CMakeProjectManager( QObject* parent, const QVariantList& )
    : KDevelop::IPlugin( CMakeSupportFactory::componentData(), parent ), m_builder(0)
{
    KDEV_USE_EXTENSION_INTERFACE( KDevelop::IBuildSystemManager )
    KDEV_USE_EXTENSION_INTERFACE( KDevelop::IProjectFileManager )
    KDEV_USE_EXTENSION_INTERFACE( KDevelop::ILanguageSupport )
    IPlugin* i = core()->pluginController()->pluginForExtension( "org.kdevelop.ICMakeBuilder" );
    Q_ASSERT(i);
    m_builder = i->extension<ICMakeBuilder>();
    Q_ASSERT(m_builder);

    new CodeCompletion(this, new CMakeCodeCompletionModel(), name());

    m_clickedItem=0;
    m_highlight = new CMakeHighlighting(this);
    {
        DUChainWriteLocker lock(DUChain::lock());
        m_buildstrapContext=new TopDUContext(IndexedString("buildstrap"), SimpleRange(0,0, 0,0));

        DUChain::self()->addDocumentChain(m_buildstrapContext);
        Q_ASSERT(DUChain::self()->chainForDocument(KUrl("buildstrap")));
    }

    QStringList envVars;
    QString executable;
#ifdef Q_OS_WIN
    envVars=CMakeProjectVisitor::envVarDirectories("Path");
    executable="cmake.exe";
#else
    envVars=CMakeProjectVisitor::envVarDirectories("PATH");
    executable="cmake";
#endif
    QString cmakeCmd = CMakeProjectVisitor::findFile(executable, envVars);
    m_modulePathDef=guessCMakeModulesDirectories(cmakeCmd);
    m_varsDef.insert("CMAKE_BINARY_DIR", QStringList("#[bin_dir]"));
    m_varsDef.insert("CMAKE_INSTALL_PREFIX", QStringList("#[install_dir]"));
    m_varsDef.insert("CMAKE_COMMAND", QStringList(cmakeCmd));
#ifdef Q_OS_WIN
    cmakeInitScripts << "CMakeMinGWFindMake.cmake";
    cmakeInitScripts << "CMakeMSYSFindMake.cmake";
    cmakeInitScripts << "CMakeNMakeFindMake.cmake";
    cmakeInitScripts << "CMakeVS8FindMake.cmake";
#else
    cmakeInitScripts << "CMakeUnixFindMake.cmake";
#endif
    cmakeInitScripts << "CMakeDetermineSystem.cmake";
    cmakeInitScripts << "CMakeDetermineCCompiler.cmake";
    cmakeInitScripts << "CMakeDetermineCXXCompiler.cmake";
    cmakeInitScripts << "CMakeSystemSpecificInformation.cmake";


    m_varsDef.insert("CMAKE_MODULE_PATH", m_modulePathDef);
    m_varsDef.insert("CMAKE_ROOT", QStringList(guessCMakeRoot(cmakeCmd)));

#if defined(Q_WS_X11) || defined(Q_WS_MAC)
    m_varsDef.insert("CMAKE_HOST_UNIX", QStringList("TRUE"));
    m_varsDef.insert("UNIX", QStringList("TRUE"));
    #ifdef Q_WS_X11
        m_varsDef.insert("LINUX", QStringList("TRUE"));
    #endif
    #ifdef Q_WS_MAC //NOTE: maybe should use __APPLE__
        m_varsDef.insert("APPLE", QStringList("TRUE"));
    #endif
#endif

#ifdef Q_OS_WIN
    m_varsDef.insert("WIN32", QStringList("TRUE"));
#endif
    kDebug(9042) << "modPath" << m_varsDef.value("CMAKE_MODULE_PATH") << m_modulePathDef;
}

CMakeProjectManager::~CMakeProjectManager()
{
}

KUrl CMakeProjectManager::buildDirectory(KDevelop::ProjectBaseItem *item) const
{
    KSharedConfig::Ptr cfg = item->project()->projectConfiguration();
    KConfigGroup group(cfg.data(), "CMake");
    if(!group.hasKey("CurrentBuildDir"))
    {
        kDebug(9032) << "No builddir specified for" << item->project()->name();
        return KUrl();
    }
        
    KUrl path = group.readEntry("CurrentBuildDir");
    KUrl projectPath = m_realRoot[item->project()];

    ProjectFolderItem *fi=dynamic_cast<ProjectFolderItem*>(item);
    for(; !fi && item; )
    {
        item=dynamic_cast<ProjectBaseItem*>(item->parent());
        fi=dynamic_cast<ProjectFolderItem*>(item);
    }
    if(!fi)
        return path;

    QString relative=KUrl::relativeUrl( projectPath, fi->url() );
    path.addPath(relative);

    kDebug() << "Build folder: " << path;
    return path;
}

void CMakeProjectManager::initializeProject(KDevelop::IProject* project, const KUrl& baseUrl)
{
    m_macrosPerProject[project].clear();
    m_varsPerProject[project]=m_varsDef;
    m_varsPerProject[project].insert("CMAKE_SOURCE_DIR", QStringList(baseUrl.toLocalFile(KUrl::RemoveTrailingSlash)));

    KSharedConfig::Ptr cfg = project->projectConfiguration();
    KConfigGroup group(cfg.data(), "CMake");
    if(group.hasKey("CMakeDir"))
    {
        QStringList l;
        foreach(const QString &path, group.readEntry("CMakeDir", QStringList()) )
        {
            if( QFileInfo(path).exists() )
            {
                m_modulePathPerProject[project] << path;
                l << path;
            }
        }
        if( !l.isEmpty() )
            group.writeEntry("CMakeDir", l);
        else
            group.writeEntry("CMakeDir", m_modulePathDef);
    }
    else
        group.writeEntry("CMakeDir", m_modulePathDef);

    foreach(const QString& script, cmakeInitScripts)
    {
        includeScript(CMakeProjectVisitor::findFile(script, m_modulePathPerProject[project], QStringList()), project);
    }
}

KDevelop::ProjectFolderItem* CMakeProjectManager::import( KDevelop::IProject *project )
{
    CMakeFolderItem* m_rootItem=0;
    KUrl cmakeInfoFile(project->projectFileUrl());
    cmakeInfoFile = cmakeInfoFile.upUrl();
    cmakeInfoFile.addPath("CMakeLists.txt");

    KUrl folderUrl=project->folder();
    kDebug(9042) << "found module path is" << m_modulePathDef;
    kDebug(9042) << "file is" << cmakeInfoFile.path();
    
    if ( !cmakeInfoFile.isLocalFile() )
    {
//         KMessageBox::error( ICore::self()->uiControllerInternal()->defaultMainWindow(),
//                                 i18n("Not a local file. CMake support doesn't handle remote projects") );
        kWarning() << "error. not a local file. CMake support doesn't handle remote projects";
    }
    else
    {
        KSharedConfig::Ptr cfg = project->projectConfiguration();
        KConfigGroup group(cfg.data(), "CMake");
        m_subprojectRoot[project] = folderUrl;
        
        if(group.hasKey("ProjectRoot"))
        {
            folderUrl=group.readEntry("ProjectRoot");
        }
        else
        {
            KUrl aux=folderUrl;
            for(; QFile::exists(aux.toLocalFile()+"/CMakeLists.txt"); aux=aux.upUrl())
            {
                kDebug() << "checking" << aux;
                folderUrl=aux;
            }
            group.writeEntry("ProjectRoot", folderUrl);
        }
        
        m_realRoot[project] = folderUrl;
        m_watchers[project] = new KDirWatch(project);
        m_modulePathPerProject[project]=m_modulePathDef;
        initializeProject(project, folderUrl);

        m_rootItem = new CMakeFolderItem(project, folderUrl.url(), 0 );
        m_rootItem->setProjectRoot(true);

        m_folderPerUrl[folderUrl]=m_rootItem;
        KUrl cachefile=buildDirectory(m_rootItem);
        cachefile.addPath("CMakeCache.txt");
        m_projectCache[project]=readCache(cachefile);
        connect(m_watchers[project], SIGNAL(dirty(const QString&)), this, SLOT(dirtyFile(const QString&)));
    }
    return m_rootItem;
}


void CMakeProjectManager::includeScript(const QString& file, KDevelop::IProject * project)
{
    kDebug(9042) << "Running cmake script: " << file;
    CMakeFileContent f = CMakeListsParser::readCMakeFile(file);
    if(f.isEmpty())
    {
        kDebug() << "There is no such file: " << file;
        return;
    }

    VariableMap *vm=&m_varsPerProject[project];
    MacroMap *mm=&m_macrosPerProject[project];
    vm->insert("CMAKE_CURRENT_BINARY_DIR", QStringList(vm->value("CMAKE_BINARY_DIR")[0]));
    vm->insert("CMAKE_CURRENT_LIST_FILE", QStringList(file));

    CMakeProjectVisitor v(file, m_buildstrapContext);
    v.setCacheValues( &m_projectCache[project] );
    v.setVariableMap(vm);
    v.setMacroMap(mm);
    v.setModulePath(m_modulePathPerProject[project]);
    v.walk(f, 0);

    vm->remove("CMAKE_CURRENT_LIST_FILE");
    vm->remove("CMAKE_CURRENT_SOURCE_DIR");
    vm->remove("CMAKE_CURRENT_BINARY_DIR");

    m_watchers[project]->addFile(file);
}

QStringList removeMatches(const QString& exp, const QStringList& orig)
{
    QStringList ret;
    QRegExp rx(exp);
    foreach(const QString& str, orig)
    {
        if(rx.indexIn(str)<0)
            ret.append(str);
    }
    return ret;
}

QList<KDevelop::ProjectFolderItem*> CMakeProjectManager::parse( KDevelop::ProjectFolderItem* item )
{
    QList<KDevelop::ProjectFolderItem*> folderList;
    CMakeFolderItem* folder = dynamic_cast<CMakeFolderItem*>( item );

    QStringList entries = QDir( item->url().toLocalFile() ).entryList( QDir::AllEntries | QDir::NoDotAndDotDot );
    entries = removeMatches("\\w*~$|\\w*\\.bak$", entries);
    if ( folder && folder->type()==KDevelop::ProjectBaseItem::BuildFolder)
    {
        m_folderPerUrl[item->url()]=folder;
        Q_ASSERT(m_folderPerUrl[item->url()]);

        kDebug(9042) << "parse:" << folder->url();
        KUrl cmakeListsPath(folder->url());
        cmakeListsPath.addPath("CMakeLists.txt");

        VariableMap *vm=&m_varsPerProject[item->project()];
        MacroMap *mm=&m_macrosPerProject[item->project()];
        CMakeFileContent f = CMakeListsParser::readCMakeFile(cmakeListsPath.toLocalFile());
        m_watchers[item->project()]->addFile(cmakeListsPath.toLocalFile());

        if(f.isEmpty())
        {
            kDebug() << "There is no" << cmakeListsPath;
            return folderList;
        }
        kDebug(9042) << "Adding cmake: " << cmakeListsPath << " to the model";

        m_watchers[item->project()]->addFile(cmakeListsPath.toLocalFile());
        QString binDir=KUrl::relativePath(m_realRoot[folder->project()].toLocalFile(), folder->url().toLocalFile());
        if(binDir.startsWith("./"))
            binDir=binDir.remove(0, 2);
        QString currentBinDir=vm->value("CMAKE_BINARY_DIR")[0]+binDir;
        
        vm->insert("CMAKE_CURRENT_BINARY_DIR", QStringList(currentBinDir));
        vm->insert("CMAKE_CURRENT_LIST_FILE", QStringList(cmakeListsPath.toLocalFile(KUrl::RemoveTrailingSlash)));
        vm->insert("CMAKE_CURRENT_SOURCE_DIR", QStringList(folder->url().toLocalFile(KUrl::RemoveTrailingSlash)));

        kDebug(9042) << "currentBinDir" << KUrl(vm->value("CMAKE_BINARY_DIR")[0]) << vm->value("CMAKE_CURRENT_BINARY_DIR");
        
    #ifdef CMAKEDEBUGVISITOR
        CMakeAstDebugVisitor dv;
        dv.walk(cmakeListsPath.toLocalFile(), f, 0);
    #endif

        ReferencedTopDUContext curr;
        if(dynamic_cast<CMakeFolderItem*>(folder->parent()))
        {
            curr=dynamic_cast<CMakeFolderItem*>(folder->parent())->topDUContext();
        }
        else
            curr=m_buildstrapContext;
        CMakeProjectVisitor v(folder->url().toLocalFile(KUrl::RemoveTrailingSlash), curr);
        v.setCacheValues(&m_projectCache[item->project()]);
        v.setVariableMap(vm);
        v.setMacroMap(mm);
        v.setModulePath(m_modulePathPerProject[item->project()]);
        v.setDefinitions(folder->definitions());
        v.walk(f, 0);
        folder->setTopDUContext(v.context());
        vm->remove("CMAKE_CURRENT_LIST_FILE");
        vm->remove("CMAKE_CURRENT_SOURCE_DIR");
        vm->remove("CMAKE_CURRENT_BINARY_DIR");

        /*{
        kDebug() << "dumpiiiiiing" << folder->url();
        KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
        KDevelop::DumpChain dump;
        dump.dump( v.context(), false);
        }*/

        if(folder->text()=="/" && !v.projectName().isEmpty())
        {
            folder->setText(v.projectName());
        }

        KUrl subroot=m_subprojectRoot[item->project()];
        
        foreach (const QString& subf, v.subdirectories())
        {
            if(subf.isEmpty()) //This would not be necessary if we didn't parse the wrong lines
                continue;
            KUrl path(folder->url());
            path.addPath(subf);
            
            kDebug(9042) << "Found subdir " << path << "which should be into" << subroot;
            if(subroot.isParentOf(path) || path.isParentOf(subroot))
            {
                if(entries.contains(subf))
                {
                    entries.removeAll(subf);
                }
                
                CMakeFolderItem* a = new CMakeFolderItem( item->project(), subf, folder );
                a->setUrl(path);
                a->setDefinitions(v.definitions());
                folderList.append( a );
            }
        }
        
//         if(folderList.isEmpty() && path.isParentOf(item->url()))
//             kDebug() << "poor guess";
        
        QStringList directories;
        directories += folder->url().toLocalFile(KUrl::RemoveTrailingSlash);
        directories += currentBinDir;

        foreach(const QString& s, v.includeDirectories())
        {
            QString dir(s);
            if(!s.startsWith('/') && !s.startsWith("#["))
            {
                KUrl path=folder->url();
                path.addPath(s);
                dir=path.toLocalFile();
            }
            if(!directories.contains(dir))
                directories.append(dir);
        }
        folder->setIncludeDirectories(directories);
        folder->setDefinitions(v.definitions());

        foreach ( const QString& t, v.targets())
        {
            QStringList dependencies=v.targetDependencies(t);
            if(!dependencies.isEmpty()) //Just to remove verbosity
            {
                KDevelop::ProjectTargetItem* targetItem;

                switch(v.targetType(t))
                {
                    case CMakeProjectVisitor::Library:
                        targetItem = new CMakeLibraryTargetItem( item->project(), t, folder, v.declarationsPerTarget()[t] );
                        break;
                    case CMakeProjectVisitor::Executable:
                        targetItem = new CMakeExecutableTargetItem( item->project(), t, folder, v.declarationsPerTarget()[t] );
                        break;
                    case CMakeProjectVisitor::Test:
                        targetItem = new CMakeTestTargetItem( item->project(), t, folder, v.declarationsPerTarget()[t] );
                        break;
                }

                foreach( const QString & sFile, dependencies )
                {
                    if(sFile.isEmpty())
                        continue;

                    KUrl sourceFile(sFile);
                    if(sourceFile.isRelative()) {
                        sourceFile = folder->url();
                        sourceFile.adjustPath( KUrl::RemoveTrailingSlash );
                        sourceFile.addPath( sFile );
                    }

                    if( entries.contains( sourceFile.fileName() ) )
                    {
                        entries.removeAll( sourceFile.fileName() );
                    }

                    new KDevelop::ProjectFileItem( item->project(), sourceFile, targetItem );
                    item->project()->addToFileSet( KDevelop::IndexedString( sourceFile ) );
                    kDebug(9042) << "..........Adding:" << sourceFile;
                }
                m_targets.append(targetItem);
            }
        }
    }

    foreach( const QString& entry, entries )
    {
        if( item->hasFileOrFolder( entry ) )
            continue;

        KUrl folderurl = item->url();
        folderurl.addPath( entry );

        KUrl cache=folderurl;
        cache.addPath("CMakeCache.txt");
        if( QFileInfo( folderurl.toLocalFile() ).isDir())
        {
            if(!QFile::exists(cache.toLocalFile()))
                folderList.append(new KDevelop::ProjectFolderItem( item->project(), folderurl, item ));
        }
        else
        {
            new KDevelop::ProjectFileItem( item->project(), folderurl, item );
            item->project()->addToFileSet( KDevelop::IndexedString( folderurl ) );
        }
    }
    return folderList;
}

QList<KDevelop::ProjectTargetItem*> CMakeProjectManager::targets() const
{
    return m_targets;
}

KUrl::List resolveSystemDirs(KDevelop::IProject* project, const QStringList& dirs)
{
    KSharedConfig::Ptr cfg = project->projectConfiguration();
    KConfigGroup group(cfg.data(), "CMake");
    
    KUrl binurl=group.readEntry("CurrentBuildDir");
    KUrl insturl=group.readEntry("CurrentInstallDir");
    
    QString bindir=binurl.toLocalFile(KUrl::AddTrailingSlash);
    QString instdir=insturl.toLocalFile(KUrl::AddTrailingSlash);

    KUrl::List newList;
    foreach(const QString& _s, dirs)
    {
        QString s=_s;
//         kDebug(9042) << "replace? " << s;
        if(s.startsWith(QString::fromUtf8("#[bin_dir]")))
        {
            s=s.replace("#[bin_dir]", bindir);
        }
        else if(s.startsWith(QString::fromUtf8("#[install_dir]")))
        {
            s=s.replace("#[install_dir]", instdir);
        }
//         kDebug(9042) << "adding " << s;
        newList.append(KUrl(s));
    }
    return newList;
}

KUrl::List CMakeProjectManager::includeDirectories(KDevelop::ProjectBaseItem *item) const
{
    CMakeFolderItem* folder=0;
    kDebug(9042) << "Querying inc dirs for " << item;
    while(!folder && item)
    {
        folder = dynamic_cast<CMakeFolderItem*>( item );
        item = static_cast<KDevelop::ProjectBaseItem*>(item->parent());
        kDebug(9042) << "Looking for a folder: " << folder << item;
    }

    if(!folder)
        return KUrl::List();

    kDebug(9042) << "Include directories! -- before" << folder->includeDirectories();
    KUrl::List l = resolveSystemDirs(folder->project(), folder->includeDirectories());
    kDebug(9042) << "Include directories!" << l;
    return l;
}

QHash< QString, QString > CMakeProjectManager::defines(KDevelop::ProjectBaseItem *item ) const
{
    CMakeFolderItem* folder=0;
    kDebug(9042) << "Querying defines dirs for " << item;
    while(!folder)
    {
        folder = dynamic_cast<CMakeFolderItem*>( item );
        item = static_cast<KDevelop::ProjectBaseItem*>(item->parent());
        kDebug(9042) << "Looking for a folder: " << folder << item;
    }

    if(!folder)
        return QHash<QString, QString>();

    return folder->definitions();
}

KDevelop::IProjectBuilder * CMakeProjectManager::builder(KDevelop::ProjectFolderItem *) const
{
    Q_ASSERT(m_builder);
    return m_builder;
}

QString CMakeProjectManager::guessCMakeShare(const QString& cmakeBin)
{
    KUrl bin(cmakeBin);
    bin=bin.upUrl();
    bin=bin.upUrl();
    return bin.toLocalFile(KUrl::RemoveTrailingSlash);
}

QString CMakeProjectManager::guessCMakeRoot(const QString & cmakeBin)
{
    QString ret;
    KUrl bin(guessCMakeShare(cmakeBin));

    QString version=executeProcess(cmakeBin, QStringList("--version"));
    QRegExp rx("[a-z* ]*([0-9.]*)-[0-9]*");
    rx.indexIn(version);
    QString versionNumber = rx.capturedTexts()[1];

    bin.cd(QString("share/cmake-%1").arg(versionNumber));

    ret=bin.toLocalFile(KUrl::RemoveTrailingSlash);
    QDir d(ret);
    if(!d.exists(ret))
    {
        KUrl std(bin);
        std = std.upUrl();
        std.cd("cmake/");
        ret=std.toLocalFile(KUrl::RemoveTrailingSlash);
        bin = std;
    }

    kDebug(9042) << "guessing: " << bin.toLocalFile(KUrl::RemoveTrailingSlash);
    return ret;
}

QStringList CMakeProjectManager::guessCMakeModulesDirectories(const QString& cmakeBin)
{
    return QStringList(guessCMakeRoot(cmakeBin)+"/Modules");
}

/*void CMakeProjectManager::parseOnly(KDevelop::IProject* project, const KUrl &url)
{
    kDebug(9042) << "Looking for" << url << " to regenerate";

    KUrl cmakeListsPath(url);
    cmakeListsPath.addPath("CMakeLists.txt");

    VariableMap *vm=&m_varsPerProject[project];
    MacroMap *mm=&m_macrosPerProject[project];

    CMakeFileContent f = CMakeListsParser::readCMakeFile(cmakeListsPath.toLocalFile());
    if(f.isEmpty())
    {
        kDebug() << "There is no" << cmakeListsPath;
        return;
    }

    QString currentBinDir=KUrl::relativeUrl(m_realRoot[project], url);
    vm->insert("CMAKE_CURRENT_BINARY_DIR", QStringList(vm->value("CMAKE_BINARY_DIR")[0]+currentBinDir));
    vm->insert("CMAKE_CURRENT_LIST_FILE", QStringList(cmakeListsPath.toLocalFile(KUrl::RemoveTrailingSlash)));
    vm->insert("CMAKE_CURRENT_SOURCE_DIR", QStringList(url.toLocalFile(KUrl::RemoveTrailingSlash)));
    CMakeProjectVisitor v(url.toLocalFile(), missingtopcontext);
    v.setCacheValues(m_projectCache[project]);
    v.setVariableMap(vm);
    v.setMacroMap(mm);
    v.setModulePath(m_modulePathPerProject[project]);
    v.walk(f, 0);
    vm->remove("CMAKE_CURRENT_LIST_FILE");
    vm->remove("CMAKE_CURRENT_SOURCE_DIR");
    vm->remove("CMAKE_CURRENT_BINARY_DIR");
}*/

//Copied from ImportJob
void CMakeProjectManager::reimport(CMakeFolderItem* fi)
{
    QQueue< QList<KDevelop::ProjectFolderItem*> > workQueue;
    QList<KDevelop::ProjectFolderItem*> initial;
    initial.append( fi );
    workQueue.enqueue( initial );

    while( workQueue.count() > 0 )
    {
        QList<KDevelop::ProjectFolderItem*> front = workQueue.dequeue();
        Q_FOREACH( KDevelop::ProjectFolderItem* _item, front )
        {
            QList<KDevelop::ProjectFolderItem*> workingList = parse( _item );
            if( workingList.count() > 0 )
                workQueue.enqueue( workingList );
        }
    }
}

void CMakeProjectManager::dirtyFile(const QString & dirty)
{
    KUrl dir(dirty);
    dir=dir.upUrl();
    dir.adjustPath(KUrl::RemoveTrailingSlash);
    KUrl dir2(dir);
    dir2.adjustPath(KUrl::AddTrailingSlash);

    CMakeFolderItem *it=0;
    if(m_folderPerUrl.contains(dir))
        it=m_folderPerUrl[dir];
    else if(m_folderPerUrl.contains(dir2))
        it=m_folderPerUrl[dir2];
    else
        kDebug(9032) << "Couldn't find the item for" << dir << dir2 << m_folderPerUrl;
    kDebug(9042) << dir << " is dirty" << it << m_folderPerUrl;
    Q_ASSERT(it);
    KDevelop::IProject* proj=it->project();
    KUrl projectBaseUrl=m_realRoot[proj];
    projectBaseUrl.adjustPath(KUrl::RemoveTrailingSlash);

    if(KUrl(dirty).fileName() == "CMakeLists.txt" && dir!=projectBaseUrl)
    {
#if 0
        KUrl relative=KUrl::relativeUrl(projectBaseUrl, dir);
        initializeProject(proj, dir);
        KUrl current=projectBaseUrl;
        QStringList subs=relative.toLocalFile().split("/");
        subs.append(QString());
        for(; !subs.isEmpty(); current.cd(subs.takeFirst()))
        {
            parseOnly(proj, current);
        }
#endif
        QStandardItem *parent=it->parent();
        parent->removeRow(it->row());
        CMakeFolderItem* fi=new CMakeFolderItem( proj, dir.toLocalFile(), parent);
        reimport(fi);
        m_folderPerUrl[dir]=fi;
    }
    else if(it)
    {
        proj->reloadModel();
    }
    else
    {
        foreach(KDevelop::IProject* project, m_watchers.uniqueKeys())
        {
            if(m_watchers[project]->contains(dirty))
                project->reloadModel();
        }
    }
}

QList< KDevelop::ProjectTargetItem * > CMakeProjectManager::targets(KDevelop::ProjectFolderItem * folder) const
{
    return folder->targetList();
}

QString CMakeProjectManager::name() const
{
    return "CMake";
}

KDevelop::ParseJob * CMakeProjectManager::createParseJob(const KUrl &)
{
    return 0;
}

KDevelop::ILanguage * CMakeProjectManager::language()
{
    return core()->languageController()->language(name());
}

const KDevelop::ICodeHighlighting* CMakeProjectManager::codeHighlighting() const
{
    return m_highlight;
}

ContextMenuExtension CMakeProjectManager::contextMenuExtension( KDevelop::Context* context )
{
    if( context->type() != KDevelop::Context::ProjectItemContext )
        return IPlugin::contextMenuExtension( context );

    KDevelop::ProjectItemContext* ctx = dynamic_cast<KDevelop::ProjectItemContext*>( context );
    QList<KDevelop::ProjectBaseItem*> items = ctx->items();

    if( items.isEmpty() )
        return IPlugin::contextMenuExtension( context );

    ContextMenuExtension menuExt;
    if(items.count()==1 && dynamic_cast<DUChainAttatched*>(items.first()))
    {
        m_clickedItem = items.first();
        KAction* action = new KAction( i18n( "Jump to target definition" ), this );
        connect( action, SIGNAL( triggered() ), this, SLOT( jumpToDeclaration() ) );
        menuExt.addAction( ContextMenuExtension::ProjectGroup, action );
    }
    return menuExt;
}

void CMakeProjectManager::jumpToDeclaration()
{
    DUChainAttatched* du=dynamic_cast<DUChainAttatched*>(m_clickedItem);
    if(du)
    {
        KTextEditor::Cursor c;
        KUrl url;
        {
            KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
            Declaration* decl = du->declaration().data();
            if(!decl)
                return;
            c = decl->range().start.textCursor();
            url = decl->url().toUrl();
        }
        
        ICore::self()->documentController()->openDocument(url, c);
    }
}

CacheValues CMakeProjectManager::readCache(const KUrl &path)
{
    QFile file(path.toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        kDebug() << "error. Could not find the file";
        return CacheValues();
    }

    CacheValues ret;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if(!line.isEmpty() && line[0].isLetter()) //it is a variable
        {
            CacheLine c;
            c.readLine(line);
            if(c.flag().isEmpty())
                ret[c.name()]=c.value();
//             kDebug(9042) << "Cache line" << line << c.name();
        }
    }
    return ret;
}


#include "cmakemanager.moc"
