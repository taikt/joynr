/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2017 BMW Car IT GmbH
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
package io.joynr.messaging.bounceproxy.controller.directory.inmemory;

import io.joynr.messaging.bounceproxy.controller.directory.AbstractBounceProxyControllerDirectoriesModule;
import io.joynr.messaging.bounceproxy.controller.directory.BounceProxyDirectory;
import io.joynr.messaging.bounceproxy.controller.directory.ChannelDirectory;

/**
 * Module to bind directory classes to in-memory implementations.
 * 
 * @author christina.strobel
 *
 */
public class InMemoryModule extends AbstractBounceProxyControllerDirectoriesModule {

    @Override
    protected void configure() {
        bind(BounceProxyDirectory.class).to(InMemoryBounceProxyDirectory.class);
        bind(ChannelDirectory.class).to(InMemoryChannelDirectory.class);
    }

}
