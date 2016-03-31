package io.joynr.capabilities.directory;

/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2014 BMW Car IT GmbH
 * %%
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * #L%
 */

import com.google.common.collect.Lists;
import com.google.inject.Inject;
import com.google.inject.name.Named;
import io.joynr.capabilities.CapabilityEntry;
import io.joynr.capabilities.CapabilityEntryPersisted;
import io.joynr.capabilities.InProcessCapabilitiesProvisioning;
import io.joynr.endpoints.JoynrMessagingEndpointAddressPersisted;
import io.joynr.messaging.MessagingPropertyKeys;
import io.joynr.runtime.SystemServicesSettings;
import joynr.system.Discovery;
import joynr.system.RoutingTypes.Address;
import joynr.types.ProviderQos;
import joynr.types.ProviderScope;
import joynr.types.Version;

import java.util.Collection;
import java.util.List;

/**
 * Provisioning with CapabilityEntryPersisted to store provisioned entry in global capabilities directory
 */
public class DirectoryCapabilitiesProvisioning extends InProcessCapabilitiesProvisioning {

    private String discoveryProviderParticipantId;
    private String systemServicesDomain;
    private String localChannelId;

    @Inject
    public DirectoryCapabilitiesProvisioning(@Named(SystemServicesSettings.PROPERTY_CC_DISCOVERY_PROVIDER_PARTICIPANT_ID) String discoveryProviderParticipantId,
                                             @Named(SystemServicesSettings.PROPERTY_SYSTEM_SERVICES_DOMAIN) String systemServicesDomain,
                                             @Named(SystemServicesSettings.PROPERTY_CC_MESSAGING_ADDRESS) Address discoveryProviderAddress,
                                             @Named(MessagingPropertyKeys.CHANNELID) String localChannelId) {
        super(discoveryProviderParticipantId, systemServicesDomain, discoveryProviderAddress);
        this.discoveryProviderParticipantId = discoveryProviderParticipantId;
        this.systemServicesDomain = systemServicesDomain;
        this.localChannelId = localChannelId;
    }

    @Override
    public Collection<? extends CapabilityEntry> getCapabilityEntries() {

        List<CapabilityEntry> provisionedList = Lists.newArrayList();
        ProviderQos providerQos = new ProviderQos();
        providerQos.setScope(ProviderScope.LOCAL);
        provisionedList.add(new CapabilityEntryPersisted(new Version(),
                                                         systemServicesDomain,
                                                         Discovery.INTERFACE_NAME,
                                                         providerQos,
                                                         discoveryProviderParticipantId,
                                                         System.currentTimeMillis(),
                                                         new JoynrMessagingEndpointAddressPersisted(localChannelId)));

        return provisionedList;
    }
}
