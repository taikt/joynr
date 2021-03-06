/*
 * #%L
 * %%
 * Copyright (C) 2018 BMW Car IT GmbH
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
#include "RobustnessTestMqttConnectionReset.h"
#ifdef JOYNR_ENABLE_DLT_LOGGING
#include <dlt/dlt.h>
#endif // JOYNR_ENABLE_DLT_LOGGING

std::shared_ptr<TestInterfaceProxy> RobustnessTestMqttConnectionReset::proxy;
std::shared_ptr<ProxyBuilder<TestInterfaceProxy>> RobustnessTestMqttConnectionReset::proxyBuilder;
std::shared_ptr<JoynrClusterControllerRuntime> RobustnessTestMqttConnectionReset::runtime;
std::string RobustnessTestMqttConnectionReset::providerDomain = "joynr-robustness-test-domain";

int main(int argc, char** argv)
{
#ifdef JOYNR_ENABLE_DLT_LOGGING
    // Register app at the dlt-daemon for logging
    DLT_REGISTER_APP("JYRC", argv[0]);
#endif // JOYNR_ENABLE_DLT_LOGGING
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
