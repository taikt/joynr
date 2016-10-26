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
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "JoynrTest.h"
#include "tests/utils/MockObjects.h"
#include "runtimes/cluster-controller-runtime/JoynrClusterControllerRuntime.h"
#include "joynr/tests/testProxy.h"
#include "joynr/MessagingSettings.h"
#include "joynr/OnChangeSubscriptionQos.h"
#include "joynr/tests/testAbstractProvider.h"
#include "joynr/LibjoynrSettings.h"
#include "joynr/PrivateCopyAssign.h"

using namespace ::testing;
using namespace joynr;

ACTION_P(ReleaseSemaphore,semaphore)
{
    semaphore->notify();
}

static const std::string messagingPropertiesPersistenceFileName1(
        "End2EndBroadcastTest-runtime1-joynr.settings");
static const std::string messagingPropertiesPersistenceFileName2(
        "End2EndBroadcastTest-runtime2-joynr.settings");

namespace joynr {

class MyTestProvider : public tests::DefaulttestProvider {
public:
    void locationChanged(const joynr::types::Localisation::GpsLocation& location) override {
        tests::testAbstractProvider::locationChanged(location);
    }

    void fireLocation(
            const joynr::types::Localisation::GpsLocation& location,
            const std::vector<std::string>& partitions = std::vector<std::string>()
    ) override {
        tests::testAbstractProvider::fireLocation(location);
    }

    void fireBroadcastWithEnumOutput(
            const joynr::tests::testTypes::TestEnum::Enum& testEnum,
            const std::vector<std::string>& partitions = std::vector<std::string>()
    ) override {
        tests::testAbstractProvider::fireBroadcastWithEnumOutput(testEnum);
    }

    void fireLocationUpdate(
            const joynr::types::Localisation::GpsLocation& location,
            const std::vector<std::string>& partitions = std::vector<std::string>()
    ) override {
        tests::testAbstractProvider::fireLocationUpdate(location);
    }

    void fireEmptyBroadcast(
            const std::vector<std::string>& partitions = std::vector<std::string>()
    ) override {
        tests::testAbstractProvider::fireEmptyBroadcast();
    }

    void fireLocationUpdateWithSpeed(
            const joynr::types::Localisation::GpsLocation& location,
            const float& currentSpeed,
            const std::vector<std::string>& partitions = std::vector<std::string>()
    ) override {
        tests::testAbstractProvider::fireLocationUpdateWithSpeed(location, currentSpeed);
    }

    void fireLocationUpdateSelective(const joynr::types::Localisation::GpsLocation& location) override {
        tests::testAbstractProvider::fireLocationUpdateSelective(location);
    }

    void fireBroadcastWithByteBufferParameter(
            const joynr::ByteBuffer& byteBufferParameter,
            const std::vector<std::string>& partitions = std::vector<std::string>()
    ) override {
        tests::testAbstractProvider::fireBroadcastWithByteBufferParameter(byteBufferParameter);
    }

    void fireBroadcastWithFiltering(
            const std::string& stringOut,
            const std::vector<std::string> & stringArrayOut,
            const std::vector<joynr::tests::testTypes::TestEnum::Enum>& enumerationArrayOut,
            const joynr::types::TestTypes::TEverythingStruct& structWithStringArrayOut,
            const std::vector<joynr::types::TestTypes::TEverythingStruct> & structWithStringArrayArrayOut
    ) override {
        tests::testAbstractProvider::fireBroadcastWithFiltering(stringOut,
                                                                stringArrayOut,
                                                                enumerationArrayOut,
                                                                structWithStringArrayOut,
                                                                structWithStringArrayArrayOut);
    }
};

class End2EndBroadcastTestBase : public TestWithParam< std::tuple<std::string, std::string> > {
public:
    JoynrClusterControllerRuntime* runtime1;
    JoynrClusterControllerRuntime* runtime2;
    std::unique_ptr<Settings> settings1;
    std::unique_ptr<Settings> settings2;
    MessagingSettings messagingSettings1;
    MessagingSettings messagingSettings2;
    std::string baseUuid;
    std::string uuid;
    std::string domainName;
    Semaphore semaphore;
    Semaphore altSemaphore;
    joynr::tests::TestLocationUpdateSelectiveBroadcastFilterParameters filterParameters;
    std::shared_ptr<MockLocationUpdatedSelectiveFilter> filter;
    std::uint16_t registerProviderWait;
    std::uint16_t subscribeToAttributeWait;
    std::uint16_t subscribeToBroadcastWait;
    joynr::types::Localisation::GpsLocation gpsLocation;
    joynr::types::Localisation::GpsLocation gpsLocation2;
    joynr::types::Localisation::GpsLocation gpsLocation3;
    joynr::types::Localisation::GpsLocation gpsLocation4;

    End2EndBroadcastTestBase() :
        runtime1(nullptr),
        runtime2(nullptr),
        settings1(std::make_unique<Settings>(std::get<0>(GetParam()))),
        settings2(std::make_unique<Settings>(std::get<1>(GetParam()))),
        messagingSettings1(*settings1),
        messagingSettings2(*settings2),
        baseUuid(util::createUuid()),
        uuid( "_" + baseUuid.substr(1, baseUuid.length()-2)),
        domainName("cppEnd2EndBroadcastTest_Domain" + uuid),
        semaphore(0),
        altSemaphore(0),
        filter(new MockLocationUpdatedSelectiveFilter),
        registerProviderWait(1000),
        subscribeToAttributeWait(2000),
        subscribeToBroadcastWait(2000),
        gpsLocation(types::Localisation::GpsLocation()),
        gpsLocation2(types::Localisation::GpsLocation(
                         9.0,
                         51.0,
                         508.0,
                         types::Localisation::GpsFixEnum::MODE2D,
                         0.0,
                         0.0,
                         0.0,
                         0.0,
                         444,
                         444,
                         2)),
        gpsLocation3(types::Localisation::GpsLocation(
                         9.0,
                         51.0,
                         508.0,
                         types::Localisation::GpsFixEnum::MODE2D,
                         0.0,
                         0.0,
                         0.0,
                         0.0,
                         444,
                         444,
                         3)),
        gpsLocation4(types::Localisation::GpsLocation(
                         9.0,
                         51.0,
                         508.0,
                         types::Localisation::GpsFixEnum::MODE2D,
                         0.0,
                         0.0,
                         0.0,
                         0.0,
                         444,
                         444,
                         4)),
        providerParticipantId()

    {
        messagingSettings1.setMessagingPropertiesPersistenceFilename(
                    messagingPropertiesPersistenceFileName1);
        messagingSettings2.setMessagingPropertiesPersistenceFilename(
                    messagingPropertiesPersistenceFileName2);

        Settings integration1Settings{"test-resources/libjoynrSystemIntegration1.settings"};
        Settings::merge(integration1Settings, *settings1, false);

        runtime1 = new JoynrClusterControllerRuntime(std::move(settings1));

        Settings integration2Settings{"test-resources/libjoynrSystemIntegration2.settings"};
        Settings::merge(integration2Settings, *settings2, false);

        runtime2 = new JoynrClusterControllerRuntime(std::move(settings2));

        filterParameters.setCountry("Germany");
        filterParameters.setStartTime("4.00 pm");
    }

    void SetUp() {
        runtime1->start();
        runtime2->start();
    }

    void TearDown() {
        if (!providerParticipantId.empty()) {
            runtime1->unregisterProvider(providerParticipantId);
        }
        bool deleteChannel = true;
        runtime1->stop(deleteChannel);
        runtime2->stop(deleteChannel);

        // Delete persisted files
        std::remove(LibjoynrSettings::DEFAULT_LOCAL_CAPABILITIES_DIRECTORY_PERSISTENCE_FILENAME().c_str());
        std::remove(LibjoynrSettings::DEFAULT_MESSAGE_ROUTER_PERSISTENCE_FILENAME().c_str());
        std::remove(LibjoynrSettings::DEFAULT_SUBSCRIPTIONREQUEST_PERSISTENCE_FILENAME().c_str());
        std::remove(LibjoynrSettings::DEFAULT_PARTICIPANT_IDS_PERSISTENCE_FILENAME().c_str());
    }

    ~End2EndBroadcastTestBase(){
        delete runtime1;
        delete runtime2;
    }

private:
    std::string providerParticipantId;
    DISALLOW_COPY_AND_ASSIGN(End2EndBroadcastTestBase);

protected:
    bool usesHttpTransport() {
        std::string brokerProtocol = messagingSettings1.getBrokerUrl()
                .getBrokerChannelsBaseUrl().getProtocol();
        return (boost::iequals(brokerProtocol, "http")
                || boost::iequals(brokerProtocol, "https"));
    }

    std::shared_ptr<MyTestProvider> registerProvider() {
        auto testProvider = std::make_shared<MyTestProvider>();
        providerParticipantId = runtime1->registerProvider<tests::testProvider>(domainName, testProvider);

        // This wait is necessary, because registerProvider is async, and a lookup could occur
        // before the register has finished.
        std::this_thread::sleep_for(std::chrono::milliseconds(registerProviderWait));

        return testProvider;
    }

    std::shared_ptr<tests::testProxy> buildProxy() {
        ProxyBuilder<tests::testProxy>* testProxyBuilder
                = runtime2->createProxyBuilder<tests::testProxy>(domainName);
        DiscoveryQos discoveryQos;
        discoveryQos.setArbitrationStrategy(DiscoveryQos::ArbitrationStrategy::HIGHEST_PRIORITY);
        discoveryQos.setDiscoveryTimeoutMs(1000);
        discoveryQos.setRetryIntervalMs(250);

        std::int64_t qosRoundTripTTL = 500;

        std::shared_ptr<tests::testProxy> testProxy(testProxyBuilder
                                                   ->setMessagingQos(MessagingQos(qosRoundTripTTL))
                                                   ->setCached(false)
                                                   ->setDiscoveryQos(discoveryQos)
                                                   ->build());

        delete testProxyBuilder;
        return testProxy;
    }

    template <typename FireBroadcast, typename SubscribeTo, typename T>
    void testOneShotBroadcastSubscription(const T& expectedValue,
                                          SubscribeTo subscribeTo,
                                          FireBroadcast fireBroadcast,
                                          const std::string& broadcastName) {
        MockSubscriptionListenerOneType<T>* mockListener =
                new MockSubscriptionListenerOneType<T>();

        // Use a semaphore to count and wait on calls to the mock listener
        ON_CALL(*mockListener, onReceive(Eq(expectedValue)))
                .WillByDefault(ReleaseSemaphore(&semaphore));

        std::shared_ptr<ISubscriptionListener<T>> subscriptionListener(
                        mockListener);
        testOneShotBroadcastSubscription(subscriptionListener,
                                         subscribeTo,
                                         fireBroadcast,
                                         broadcastName,
                                         expectedValue);
    }

    template <typename FireBroadcast, typename SubscribeTo, typename ...T>
    void testOneShotBroadcastSubscription(std::shared_ptr<ISubscriptionListener<T...>> subscriptionListener,
                                          SubscribeTo subscribeTo,
                                          FireBroadcast fireBroadcast,
                                          const std::string& broadcastName,
                                          T... expectedValues) {
        std::vector<std::string> partitions({}); // TODO test with real partitions
        std::shared_ptr<MyTestProvider> testProvider = registerProvider();

        std::shared_ptr<tests::testProxy> testProxy = buildProxy();

        std::int64_t minInterval_ms = 50;
        auto subscriptionQos = std::make_shared<OnChangeSubscriptionQos>(
                    500000,   // validity_ms
                    minInterval_ms);  // minInterval_ms

        subscribeTo(testProxy.get(), subscriptionListener, subscriptionQos);

        (*testProvider.*fireBroadcast)(expectedValues..., partitions);

        // Wait for a subscription message to arrive
        ASSERT_TRUE(semaphore.waitFor(std::chrono::seconds(3)));
    }

    template <typename BroadcastFilter>
    void addFilterToTestProvider(std::shared_ptr<MyTestProvider> testProvider, std::shared_ptr<BroadcastFilter> filter)
    {
        if (filter) {
            testProvider->addBroadcastFilter(filter);
        }
    }

    void addFilterToTestProvider(std::shared_ptr<MyTestProvider> testProvider, std::nullptr_t filter)
    {
        std::ignore = testProvider;
        std::ignore = filter;
    }

    template <typename FireBroadcast, typename SubscribeTo, typename BroadcastFilterPtr, typename ...T>
    void testOneShotBroadcastSubscriptionWithFiltering(std::shared_ptr<ISubscriptionListener<T...>> subscriptionListener,
                                          SubscribeTo subscribeTo,
                                          FireBroadcast fireBroadcast,
                                          const std::string& broadcastName,
                                          BroadcastFilterPtr filter,
                                          T... expectedValues) {
        std::shared_ptr<MyTestProvider> testProvider = registerProvider();
        addFilterToTestProvider(testProvider, filter);

        std::shared_ptr<tests::testProxy> testProxy = buildProxy();

        std::int64_t minInterval_ms = 50;
        auto subscriptionQos = std::make_shared<OnChangeSubscriptionQos>(
                    500000,   // validity_ms
                    minInterval_ms);  // minInterval_ms

        subscribeTo(testProxy.get(), subscriptionListener, subscriptionQos);

        (*testProvider.*fireBroadcast)(expectedValues...);

        // Wait for a subscription message to arrive
        ASSERT_TRUE(semaphore.waitFor(std::chrono::seconds(3)));
    }
};

} // namespace joynr

