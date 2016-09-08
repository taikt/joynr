package io.joynr.proxy.invocation;

/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2016 BMW Car IT GmbH
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

import java.lang.reflect.Method;

import com.fasterxml.jackson.core.type.TypeReference;
import io.joynr.dispatcher.rpc.annotation.JoynrRpcSubscription;
import io.joynr.exceptions.JoynrIllegalStateException;
import io.joynr.proxy.Future;
import io.joynr.pubsub.SubscriptionQos;
import io.joynr.pubsub.subscription.AttributeSubscriptionListener;

/**
 * AttributeSubscribeInvocation contains the queuable information for a {@literal subscribeTo<attribute>} call
 */
public class AttributeSubscribeInvocation extends SubscriptionInvocation {
    private final AttributeSubscriptionListener<?> attributeSubscriptionListener;
    private final Class<?> attributeTypeReference;

    public AttributeSubscribeInvocation(Method method, Object[] args, Future<String> future) {
        super(future, getAttributeNameFromAnnotation(method), (SubscriptionQos) args[1]);
        if (args[0] == null || !AttributeSubscriptionListener.class.isAssignableFrom(args[0].getClass())) {
            throw new JoynrIllegalStateException("First parameter of subscribeTo... has to implement AttributeSubscriptionListener");
        }
        attributeTypeReference = getAnnotationFromMethod(method).attributeType();

        attributeSubscriptionListener = (AttributeSubscriptionListener<?>) args[0];
        if (args[1] == null || !SubscriptionQos.class.isAssignableFrom(args[1].getClass())) {
            throw new JoynrIllegalStateException("Second parameter of subscribeTo... has to be of type SubscriptionQos");
        }

        if (args.length > 2) {
            if (args[2] != null && args[2] instanceof String) {
                setSubscriptionId((String) args[2]);
            } else {
                throw new JoynrIllegalStateException("Third parameter of subscribeTo... has to be of type String");
            }
        }
    }

    private static JoynrRpcSubscription getAnnotationFromMethod(Method method) {
        JoynrRpcSubscription subscriptionAnnotation = method.getAnnotation(JoynrRpcSubscription.class);
        if (subscriptionAnnotation == null) {
            throw new JoynrIllegalStateException("SubscribeTo... methods must be annotated with JoynrRpcSubscription annotation");
        }
        return subscriptionAnnotation;
    }

    private static String getAttributeNameFromAnnotation(Method method) {
        return getAnnotationFromMethod(method).attributeName();
    }

    public AttributeSubscribeInvocation(String attributeName,
                                        Class<? extends TypeReference<?>> attributeTypeReference,
                                        AttributeSubscriptionListener<?> attributeSubscriptionListener,
                                        SubscriptionQos qos,
                                        Future<String> future) {
        super(future, attributeName, qos);
        this.attributeTypeReference = attributeTypeReference;
        this.attributeSubscriptionListener = attributeSubscriptionListener;
    }

    public AttributeSubscriptionListener<?> getAttributeSubscriptionListener() {
        return attributeSubscriptionListener;
    }

    public Class<?> getAttributeTypeReference() {
        return attributeTypeReference;
    }

    public String getAttributeName() {
        return getSubscriptionName();
    }
}
