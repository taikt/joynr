package io.joynr.generator.cpp.proxy
/*
 * !!!
 *
 * Copyright (C) 2011 - 2015 BMW Car IT GmbH
 *
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
 */

import com.google.inject.Inject
import io.joynr.generator.cpp.util.CppStdTypeUtil
import io.joynr.generator.cpp.util.JoynrCppGeneratorExtensions
import io.joynr.generator.cpp.util.TemplateBase
import io.joynr.generator.util.InterfaceTemplate
import org.franca.core.franca.FInterface

class InterfaceProxyBaseHTemplate implements InterfaceTemplate{
	@Inject	extension JoynrCppGeneratorExtensions
	@Inject extension TemplateBase
	@Inject extension CppStdTypeUtil

	override generate(FInterface serviceInterface)
'''
«val interfaceName =  serviceInterface.joynrName»
«val className = interfaceName + "ProxyBase"»
«val headerGuard = ("GENERATED_INTERFACE_"+getPackagePathWithJoynrPrefix(serviceInterface, "_")+
	"_"+interfaceName+"ProxyBase_h").toUpperCase»
«warning()»

#ifndef «headerGuard»
#define «headerGuard»

#include "joynr/PrivateCopyAssign.h"
«FOR parameterType: getRequiredIncludesFor(serviceInterface).addElements(includeForString)»
	#include «parameterType»
«ENDFOR»
#include <QSharedPointer>
#include <memory>

«getDllExportIncludeStatement()»
#include "joynr/ProxyBase.h"
#include "«getPackagePathWithJoynrPrefix(serviceInterface, "/")»/I«interfaceName»Connector.h"

«getNamespaceStarter(serviceInterface)»
class «getDllExportMacro()» «className»: virtual public joynr::ProxyBase, virtual public «getPackagePathWithJoynrPrefix(serviceInterface, "::")»::I«interfaceName»Subscription {
public:
	«className»(
			QSharedPointer<joynr::system::QtAddress> messagingAddress,
			joynr::ConnectorFactory* connectorFactory,
			joynr::IClientCache* cache,
			const std::string& domain,
			const joynr::MessagingQos& qosSettings,
			bool cached
	);

	~«className»();

	void handleArbitrationFinished(
			const std::string &participantId,
			const joynr::types::StdCommunicationMiddleware::Enum& connection
	);
	«FOR attribute: getAttributes(serviceInterface).filter[attribute | attribute.notifiable]»
		«val returnType = attribute.typeName»
		«var attributeName = attribute.joynrName»
		std::string subscribeTo«attributeName.toFirstUpper»(
					std::shared_ptr<joynr::ISubscriptionListener<«returnType»> > subscriptionListener,
					const joynr::StdSubscriptionQos& subscriptionQos);
		std::string subscribeTo«attributeName.toFirstUpper»(
					std::shared_ptr<joynr::ISubscriptionListener<«returnType»> > subscriptionListener,
					const joynr::StdSubscriptionQos& subscriptionQos,
					std::string& subcriptionId);
		void unsubscribeFrom«attributeName.toFirstUpper»(std::string& subscriptionId);
	«ENDFOR»

	«FOR broadcast: serviceInterface.broadcasts»
		«val returnTypes = broadcast.commaSeparatedOutputParameterTypes»
		«var broadcastName = broadcast.joynrName»
		«IF isSelective(broadcast)»
			std::string subscribeTo«broadcastName.toFirstUpper»Broadcast(
						const «interfaceName.toFirstUpper»«broadcastName.toFirstUpper»BroadcastFilterParameters& filterParameters,
						std::shared_ptr<joynr::ISubscriptionListener<«returnTypes»> > subscriptionListener,
						const joynr::StdOnChangeSubscriptionQos& subscriptionQos);

			std::string subscribeTo«broadcastName.toFirstUpper»Broadcast(
						const «interfaceName.toFirstUpper»«broadcastName.toFirstUpper»BroadcastFilterParameters& filterParameters,
						std::shared_ptr<joynr::ISubscriptionListener<«returnTypes»> > subscriptionListener,
						const joynr::StdOnChangeSubscriptionQos& subscriptionQos,
						std::string& subscriptionId);
		«ELSE»
			std::string subscribeTo«broadcastName.toFirstUpper»Broadcast(
						std::shared_ptr<joynr::ISubscriptionListener<«returnTypes»> > subscriptionListener,
						const joynr::StdOnChangeSubscriptionQos& subscriptionQos);

			std::string subscribeTo«broadcastName.toFirstUpper»Broadcast(
						std::shared_ptr<joynr::ISubscriptionListener<«returnTypes»> > subscriptionListener,
						const joynr::StdOnChangeSubscriptionQos& subscriptionQos,
						std::string& subscriptionId);
		«ENDIF»
		void unsubscribeFrom«broadcastName.toFirstUpper»Broadcast(std::string& subscriptionId);

	«ENDFOR»

protected:
	QSharedPointer<joynr::system::QtAddress> messagingAddress;
	I«interfaceName»Connector* connector;

private:
	DISALLOW_COPY_AND_ASSIGN(«className»);
};
«getNamespaceEnder(serviceInterface)»
#endif // «headerGuard»
'''
}
