/*
 * #%L
 * %%
 * Copyright (C) 2019 BMW Car IT GmbH
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
class SubscriptionListener {
    /**
     * Is called if publication is missed or an error occurs
     * @name SubscriptionListener#onError
     */
    public onError?: Function;

    /**
     * Is called if publication is received
     * @name SubscriptionListener#onReceive
     */
    public onReceive: Function;

    /**
     * Is called if subscription request has been successfully delivered to the provider
     * @name SubscriptionListener#onSubscribed
     */
    public onSubscribed?: Function;

    /**
     * @constructor
     *
     * @param settings.onReceive The function to call when a publication is received
     * @param settings.onError The function to call if no publication is received in the given alert interval
     *            or if some other error occurs. The error is passed as parameter.
     */
    public constructor(settings: { onReceive: Function; onError?: Function; onSubscribed?: Function }) {
        this.onSubscribed = settings.onSubscribed;

        this.onReceive = settings.onReceive;

        this.onError = settings.onError;
    }
}

export = SubscriptionListener;
