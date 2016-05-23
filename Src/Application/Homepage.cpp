#include "Homepage.h"
#include "HomepageUI.h"
#include "AppManager.h"
#include "PointShopApp.h"
#include "MeshShopApp.h"
#include "RegistrationApp.h"
#include "MeasureApp.h"
#include "ReliefApp.h"
#include "../Common/LogSystem.h"
#include "../Common/ToolKit.h"
#include "DumpInfo.h"

namespace MagicApp
{
    Homepage::Homepage() :
        mpUI(NULL)
    {
    }

    Homepage::~Homepage()
    {
        if (mpUI != NULL)
        {
            delete mpUI;
            mpUI = NULL;
        }
    }

    bool Homepage::Enter()
    {
        InfoLog << "Enter Homepage" << std::endl;
        if (mpUI == NULL)
        {
            mpUI = new HomepageUI;
        }
        mpUI->Setup();
        return true;
    }

    bool Homepage::Update(double timeElapsed)
    {
        return true;
    }

    bool Homepage::Exit()
    {
        InfoLog << "Exit Homepage" << std::endl;
        if (mpUI != NULL)
        {
            mpUI->Shutdown();
        }
        return true;
    }

    bool Homepage::KeyPressed( const OIS::KeyEvent &arg )
    {
        if (arg.key == OIS::KC_D)
        {
#if DEBUGDUMPFILE
            LoadDumpFile();
#endif
        }
        return true;
    }

#if DEBUGDUMPFILE
    void Homepage::LoadDumpFile(void)
    {
        std::string fileName;
        char filterName[] = "Dump Files(*.dump)\0*.dump\0";
        if (MagicCore::ToolKit::FileOpenDlg(fileName, filterName))
        {
            std::ifstream fin(fileName.c_str());
            int dumpApiName;
            fin >> dumpApiName;
            fin.close();
            GPP::DumpBase* dumpInfo = GPP::DumpManager::Get()->GetDumpInstance(dumpApiName);
            if (dumpInfo == NULL)
            {
                return;
            }
            dumpInfo->LoadDumpFile(fileName);
            if (dumpApiName == GPP::POINT_REGISTRATION__AlignPointPair || dumpApiName == GPP::POINT_REGISTRATION_ICP || 
                dumpApiName == GPP::POINT_REGISTRATION_ALIGNPOINTCLOUD || dumpApiName == GPP::POINT_REGISTRATION_ALIGNPOINTCLOUD_MARK)
            {
                AppManager::Get()->EnterApp(new RegistrationApp, "RegistrationApp");
                RegistrationApp* registrationApp = dynamic_cast<RegistrationApp*>(AppManager::Get()->GetApp("RegistrationApp"));
                if (registrationApp)
                {
                    registrationApp->SetDumpInfo(dumpInfo);
                }
                else
                {
                    GPPFREEPOINTER(dumpInfo);
                }
            }
            else if (dumpApiName == GPP::MESH_MEASURE_SECTION_APPROXIMATE || dumpApiName == GPP::MESH_MEASURE_SECTION_EXACT ||
                dumpApiName == GPP::MESH_MEASURE_SECTION_FAST_EXACT)
            {
                AppManager::Get()->EnterApp(new MeasureApp, "MeasureApp");
                MeasureApp* measureApp = dynamic_cast<MeasureApp*>(AppManager::Get()->GetApp("MeasureApp"));
                if (measureApp)
                {
                    measureApp->SetDumpInfo(dumpInfo);
                }
                else
                {
                    GPPFREEPOINTER(dumpInfo);
                }
            }
            else if (dumpApiName == GPP::MESH_FILTER_COMPRESSHEIGHTFIELD)
            {
                AppManager::Get()->EnterApp(new MeasureApp, "ReliefApp");
                ReliefApp* reliefApp = dynamic_cast<ReliefApp*>(AppManager::Get()->GetApp("ReliefApp"));
                if (reliefApp)
                {
                    reliefApp->SetDumpInfo(dumpInfo);
                }
                else
                {
                    GPPFREEPOINTER(dumpInfo);
                }
            }
            else if (dumpInfo->GetPointCloud() != NULL || dumpApiName == GPP::POINT_FUSE_UPDATE || dumpApiName == GPP::POINT_FUSE_EXTRACT)
            {
                AppManager::Get()->EnterApp(new PointShopApp, "PointShopApp");
                PointShopApp* pointShop = dynamic_cast<PointShopApp*>(AppManager::Get()->GetApp("PointShopApp"));
                if (pointShop)
                {
                    pointShop->SetDumpInfo(dumpInfo);
                }
                else
                {
                    GPPFREEPOINTER(dumpInfo);
                }
            }
            else if (dumpInfo->GetTriMesh() != NULL)
            {
                AppManager::Get()->EnterApp(new MeshShopApp, "MeshShopApp");
                MeshShopApp* meshShop = dynamic_cast<MeshShopApp*>(AppManager::Get()->GetApp("MeshShopApp"));
                if (meshShop)
                {
                    meshShop->SetDumpInfo(dumpInfo);
                }
                else
                {
                    GPPFREEPOINTER(dumpInfo);
                }
            }
            else
            {
                GPPFREEPOINTER(dumpInfo);
            }
        }
    }
#endif
}
