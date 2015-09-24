/*global ttl: true */

/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2015 BMW Car IT GmbH
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

(function() {
    var setupProvisionedData = function(provisioning) {
        var discoveryCapability = {
            domain : "io.joynr",
            interfaceName : "system/Discovery",
            providerQos : {
                qos : [],
                version : 0,
                priority : 1,
                isLocalOnly : false,
                onChangeSubscriptions : true
            },
            participantId : "CC.DiscoveryProvider.ParticipantId"
        };

        var routingCapability = {
            domain : "io.joynr",
            interfaceName : "system/Routing",
            providerQos : {
                qos : [],
                version : 0,
                priority : 1,
                isLocalOnly : false,
                onChangeSubscriptions : true
            },
            participantId : "CC.RoutingProvider.ParticipantId"
        };

        provisioning.capabilities = [ discoveryCapability, routingCapability ];
        return provisioning;
    };

    // AMD support
    if (typeof define === 'function' && define.amd) {
        define("joynr/provisioning/provisioning_libjoynr",
            ["joynr/provisioning/provisioning_common"], function(provisioning){
            return setupProvisionedData(provisioning);
        });
    } else {
        // expect that joynrprovisioning.common has been loaded before
        setupProvisionedData(window.joynr.provisioning);
    }
}());