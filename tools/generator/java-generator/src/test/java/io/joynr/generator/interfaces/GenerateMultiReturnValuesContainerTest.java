/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2020 BMW Car IT GmbH
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
package io.joynr.generator.interfaces;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.Map;

import org.junit.Before;
import org.junit.Test;

import io.joynr.generator.AbstractJoynrJavaGeneratorTest;

public class GenerateMultiReturnValuesContainerTest extends AbstractJoynrJavaGeneratorTest {

    @Before
    public void setup() throws Exception {
        final boolean generateProxy = true;
        final boolean generateProvider = true;
        super.setup(generateProxy, generateProvider);
    }

    @Test
    public void testGenerateMultiReturnMethod() {
        Map<String, String> result = generate("multi-out-method-test.fidl");
        assertNotNull(result);
        boolean containerFound = false;
        for (Map.Entry<String, String> entry : result.entrySet()) {
            if (entry.getKey().endsWith("Sync")) {
                assertTrue("Marker interface not added.",
                           entry.getValue().contains("implements io.joynr.dispatcher.rpc.MultiReturnValuesContainer"));
                containerFound = true;
            }
        }
        assertTrue(containerFound);
    }

}
