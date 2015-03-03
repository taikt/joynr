package io.joynr.pubsub.publication;

/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2013 BMW Car IT GmbH
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

import io.joynr.dispatcher.RequestCaller;

import java.util.List;

import joynr.SubscriptionRequest;

public interface PublicationManager {

    /**
     * @brief Adds SubscriptionRequest when the Provider is not yet registered and there is no RequestCaller as yet.
     * 
     * @param subscriptionRequest
     */
    void addSubscriptionRequest(String fromParticipantId,
                                String toParticipantId,
                                SubscriptionRequest subscriptionRequest);

    /**
     * @brief Adds the SubscriptionRequest for event or attribute
     * @param fromParticipantId
     * @param toParticipantId
     * @param subscriptionRequest
     * @param requestCaller
     */
    void addSubscriptionRequest(String fromParticipantId,
                                String toParticipantId,
                                SubscriptionRequest subscriptionRequest,
                                RequestCaller requestCaller);

    /**
     * @brief Stops the sending of publications
     * 
     * @param subscriptionId
     */
    void stopPublication(final String subscriptionId);

    /**
     * @brief Stops all publications for a provider
     * 
     * @param providerId
     */
    void stopPublicationByProviderId(String providerId);

    /**
     * @brief Called by the Dispatcher every time a provider is registered to check whether there are already
     *        subscriptionRequests waiting.
     * 
     * @param providerId
     * @param requestCaller
     */
    void restoreQueuedSubscription(final String providerId, RequestCaller requestCaller);

    /**
     * @brief Call passed through from ProviderListener when an attribute on the provider is changed.
     * @param subscriptionId
     * @param value
     */
    void attributeValueChanged(String subscriptionId, Object value);

    void broadcastOccurred(String subscriptionId, List<BroadcastFilter> filters, Object... values);
}
