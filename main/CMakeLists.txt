set(PROJECT_NAME "modbus_tcp_master")

idf_component_register(	SRC_DIRS    		${CMAKE_CURRENT_LIST_DIR}/src
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/common
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/goose
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/iec61850/client
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/iec61850/common
					        ${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/iec61850/server/impl
											 			       					      ${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/iec61850/server/mms_mapping
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/iec61850/server/model
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/logging
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/asn1
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_acse
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_client
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_common
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_cotp
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_mms/asn1c
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_mms/client
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_mms/common
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_mms/server
												  						       			${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_presentation
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_server
						${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/mms/iso_session
						#${CMAKE_CURRENT_LIST_DIR}/libiec61850/src/sampled_values
						

                       	INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include

				PRIV_INCLUDE_DIRS 						${CMAKE_CURRENT_LIST_DIR}/include_private
	"."
							

				REQUIRES 	esp_netif
						vfs
						hal
						unity
						newlib
						driver
						pthread
						protocol_examples_common
						nvs_flash
						esp_eth
						

				PRIV_REQUIRES	newlib

)


target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")