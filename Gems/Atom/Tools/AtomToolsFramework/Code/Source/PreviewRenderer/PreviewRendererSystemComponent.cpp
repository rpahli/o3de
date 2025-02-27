/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <PreviewRenderer/PreviewRendererSystemComponent.h>

namespace AtomToolsFramework
{
    void PreviewRendererSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<PreviewRendererSystemComponent, AZ::Component>()
                ->Version(0);

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<PreviewRendererSystemComponent>("PreviewRendererSystemComponent", "System component that manages a global PreviewRenderer.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    void PreviewRendererSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("PreviewRendererSystem"));
    }

    void PreviewRendererSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("PreviewRendererSystem"));
    }

    void PreviewRendererSystemComponent::Init()
    {
    }

    void PreviewRendererSystemComponent::Activate()
    {
        AzFramework::AssetCatalogEventBus::Handler::BusConnect();
        AzFramework::ApplicationLifecycleEvents::Bus::Handler::BusConnect();
        PreviewRendererSystemRequestBus::Handler::BusConnect();
    }

    void PreviewRendererSystemComponent::Deactivate()
    {
        PreviewRendererSystemRequestBus::Handler::BusDisconnect();
        AzFramework::ApplicationLifecycleEvents::Bus::Handler::BusDisconnect();
        AzFramework::AssetCatalogEventBus::Handler::BusDisconnect();
        m_previewRenderer.reset();
    }

    void PreviewRendererSystemComponent::OnCatalogLoaded([[maybe_unused]] const char* catalogFile)
    {
        AZ::TickBus::QueueFunction([this](){
            m_previewRenderer.reset(aznew AtomToolsFramework::PreviewRenderer(
                "PreviewRendererSystemComponent Preview Scene", "PreviewRendererSystemComponent Preview Pipeline"));
        });
    }

    void PreviewRendererSystemComponent::OnApplicationAboutToStop()
    {
        m_previewRenderer.reset();
    }
} // namespace AtomToolsFramework
