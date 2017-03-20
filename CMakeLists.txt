cmake_minimum_required(VERSION 3.6)
project(IceKnifeLib)

set(CMAKE_CXX_STANDARD 11)

set(Include_Path ${CMAKE_SOURCE_DIR}/include )
set(OpenSSL_Include_Path ${CMAKE_SOURCE_DIR}/Third/openssl/include )
set(OpenSSL_Libs_Dir ${CMAKE_SOURCE_DIR}/Third/openssl/libs )


include_directories(${Include_Path}) #自己的头文件
include_directories(${OpenSSL_Include_Path}) #openssl的头文件文件夹
link_libraries(${OpenSSL_Libs_Dir}/libcrypto.a)
link_libraries(${OpenSSL_Libs_Dir}/libssl.a)


set(SOURCE_FILES
        library.cpp
        library.h
        include/PreDefine.h
        include/LVcDefine.h
        include/CPubFuncUtil.h
        include/CHttp.h
        include/CUtil.h
        include/Base64.h
        include/URLDecoder.h
        include/SHA1.h
        include/sqlite/sqlite3.h
        include/CDbControl.h
        include/CDialVpn.h
        include/CAesUtil.h
        include/CRsaUtil.h
        include/json/assertions.h
        include/json/autolink.h
        include/json/config.h
        include/json/features.h
        include/json/forwards.h
        include/json.h
        include/json/reader.h
        include/json/value.h
        include/json/writer.h
        include/json/version.h
        include/ConcurrentMap.h
        include/ConcurrentVector.h
        include/INotification.h

        Public/CPubFuncUtil.cpp
        Public/CUtil.cpp
        Public/CHttp/CHttp.cpp
        Public/URLDecoder.cpp
        Public/tinyxml/tinyxml2.h
        Public/tinyxml/tinyxml2.cpp

        Public/json/allocator.h
        Public/json/json_reader.cpp
        Public/json/json_tool.h
        Public/json/json_value.cpp
        Public/json/json_writer.cpp

        Public/encode/md5/md5.cpp
        Public/encode/md5/md5.h
        Public/encode/CSHA1/SHA1.cpp
        Public/encode/aes/CAesUtil.cpp
        Public/encode/rsa/CRsaUtil.cpp

        Public/CDbControl/CDbControl.cpp

        Public/sqlite/sqlite3.c

        Public/DialVps/CDialVpn.cpp





        #openssl
        Third/openssl/include/internal/bio.h
        Third/openssl/include/internal/comp.h
        Third/openssl/include/internal/conf.h
        Third/openssl/include/internal/constant_time_locl.h
        Third/openssl/include/internal/dane.h
        Third/openssl/include/internal/dso.h
        Third/openssl/include/internal/err.h
        Third/openssl/include/internal/numbers.h
        Third/openssl/include/internal/o_dir.h
        Third/openssl/include/internal/o_str.h
        Third/openssl/include/internal/thread_once.h


        Third/openssl/include/openssl/__DECC_INCLUDE_EPILOGUE.H
        Third/openssl/include/openssl/__DECC_INCLUDE_PROLOGUE.H
        Third/openssl/include/openssl/aes.h
        Third/openssl/include/openssl/asn1.h
        Third/openssl/include/openssl/asn1_mac.h
        Third/openssl/include/openssl/asn1t.h
        Third/openssl/include/openssl/async.h
        Third/openssl/include/openssl/bio.h
        Third/openssl/include/openssl/blowfish.h
        Third/openssl/include/openssl/bn.h
        Third/openssl/include/openssl/buffer.h
        Third/openssl/include/openssl/camellia.h
        Third/openssl/include/openssl/cast.h
        Third/openssl/include/openssl/cmac.h
        Third/openssl/include/openssl/cms.h
        Third/openssl/include/openssl/comp.h
        Third/openssl/include/openssl/conf.h
        Third/openssl/include/openssl/conf_api.h
        Third/openssl/include/openssl/crypto.h
        Third/openssl/include/openssl/ct.h
        Third/openssl/include/openssl/des.h
        Third/openssl/include/openssl/dh.h
        Third/openssl/include/openssl/dsa.h
        Third/openssl/include/openssl/dtls1.h
        Third/openssl/include/openssl/e_os2.h
        Third/openssl/include/openssl/ebcdic.h
        Third/openssl/include/openssl/ec.h
        Third/openssl/include/openssl/ecdh.h
        Third/openssl/include/openssl/ecdsa.h
        Third/openssl/include/openssl/engine.h
        Third/openssl/include/openssl/err.h
        Third/openssl/include/openssl/evp.h
        Third/openssl/include/openssl/hmac.h
        Third/openssl/include/openssl/idea.h
        Third/openssl/include/openssl/kdf.h
        Third/openssl/include/openssl/lhash.h
        Third/openssl/include/openssl/md2.h
        Third/openssl/include/openssl/md4.h
        Third/openssl/include/openssl/md5.h
        Third/openssl/include/openssl/mdc2.h
        Third/openssl/include/openssl/modes.h
        Third/openssl/include/openssl/obj_mac.h
        Third/openssl/include/openssl/objects.h
        Third/openssl/include/openssl/ocsp.h
        Third/openssl/include/openssl/opensslconf.h
        Third/openssl/include/openssl/opensslconf.h.in
        Third/openssl/include/openssl/opensslconf副本.h
        Third/openssl/include/openssl/opensslv.h
        Third/openssl/include/openssl/ossl_typ.h
        Third/openssl/include/openssl/pem.h
        Third/openssl/include/openssl/pem2.h
        Third/openssl/include/openssl/pkcs7.h
        Third/openssl/include/openssl/pkcs12.h
        Third/openssl/include/openssl/rand.h
        Third/openssl/include/openssl/rc2.h
        Third/openssl/include/openssl/rc4.h
        Third/openssl/include/openssl/rc5.h
        Third/openssl/include/openssl/ripemd.h
        Third/openssl/include/openssl/rsa.h
        Third/openssl/include/openssl/safestack.h
        Third/openssl/include/openssl/seed.h
        Third/openssl/include/openssl/sha.h
        Third/openssl/include/openssl/srp.h
        Third/openssl/include/openssl/srtp.h
        Third/openssl/include/openssl/ssl.h
        Third/openssl/include/openssl/ssl2.h
        Third/openssl/include/openssl/ssl3.h
        Third/openssl/include/openssl/stack.h
        Third/openssl/include/openssl/symhacks.h
        Third/openssl/include/openssl/tls1.h
        Third/openssl/include/openssl/ts.h
        Third/openssl/include/openssl/txt_db.h
        Third/openssl/include/openssl/ui.h
        Third/openssl/include/openssl/whrlpool.h
        Third/openssl/include/openssl/x509.h
        Third/openssl/include/openssl/x509_vfy.h
        Third/openssl/include/openssl/x509v3.h


        include/NotificationCenter.h
        include/ST_Notification.h include/CAnyList.h include/CBaseTypeUtil.h Public/CBaseTypeUtil.cpp)
add_library(IceKnifeLib ${SOURCE_FILES})
