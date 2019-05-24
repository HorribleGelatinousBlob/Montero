// Copyright (c) 2017-2018, The NERVA Project
// Copyright (c) 2017-2018, The Masari Project
// Copyright (c) 2014-2018, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <stdexcept>
#include <string>
#include <set>
#include <boost/uuid/uuid.hpp>

#define CRYPTONOTE_DNS_TIMEOUT_MS                       20000

#define CRYPTONOTE_MAX_BLOCK_NUMBER                     500000000
#define CRYPTONOTE_MAX_BLOCK_SIZE                       500000000  // block header blob limit, never used!
#define CRYPTONOTE_MAX_TX_SIZE                          1000000000
#define CRYPTONOTE_PUBLIC_ADDRESS_TEXTBLOB_VER          0
#define CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW            10
#define CURRENT_TRANSACTION_VERSION                     1
#define CURRENT_BLOCK_MAJOR_VERSION                     1
#define CURRENT_BLOCK_MINOR_VERSION                     1
#define CRYPTONOTE_DEFAULT_TX_SPENDABLE_AGE             10

#define BULLETPROOF_MAX_OUTPUTS                         16
#define BULLETPROOF_SIMPLE_FORK_HEIGHT                  1
#define BULLETPROOF_FULL_FORK_HEIGHT                    2

#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW               12
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT              60*5

#define BLOCK_REWARD                                    ((uint64_t)10000000)
#define GENESIS_BLOCK_REWARD                            ((uint64_t)10000000)
#define PER_KB_BASE_FEE                                 ((uint64_t)5000)                         

#define CRYPTONOTE_REWARD_BLOCKS_WINDOW                 100
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE       300000
#define CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE          600
#define CRYPTONOTE_DISPLAY_DECIMAL_POINT                6

#define DEFAULT_MIXIN                                   4
#define DEFAULT_RINGSIZE                                DEFAULT_MIXIN + 1

#define DIFFICULTY_TARGET                               60  // seconds

#define DIFFICULTY_BLOCKS_ESTIMATE_TIMESPAN             DIFFICULTY_TARGET //just alias; used by tests
#define DIFFICULTY_WINDOW                               17
#define DIFFICULTY_CUT                                  6
#define DIFFICULTY_BLOCKS_COUNT                         DIFFICULTY_WINDOW + DIFFICULTY_CUT*2

#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V1   DIFFICULTY_TARGET * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS       1

#define BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT          10000  //by default, blocks ids count in synchronizing
#define BLOCKS_SYNCHRONIZING_DEFAULT_COUNT              20     //by default, blocks count in blocks downloading

#define CRYPTONOTE_MEMPOOL_TX_LIVETIME                  (86400*3) //seconds, three days
#define CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME   604800 //seconds, one week

#define COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT           1000

#define P2P_LOCAL_WHITE_PEERLIST_LIMIT                  1000
#define P2P_LOCAL_GRAY_PEERLIST_LIMIT                   5000

#define P2P_DEFAULT_CONNECTIONS_COUNT                   8
#define P2P_DEFAULT_HANDSHAKE_INTERVAL                  60           //secondes
#define P2P_DEFAULT_PACKET_MAX_SIZE                     50000000     //50000000 bytes maximum packet size
#define P2P_DEFAULT_PEERS_IN_HANDSHAKE                  250
#define P2P_DEFAULT_CONNECTION_TIMEOUT                  5000       //5 seconds
#define P2P_DEFAULT_PING_CONNECTION_TIMEOUT             2000       //2 seconds
#define P2P_DEFAULT_INVOKE_TIMEOUT                      60*2*1000  //2 minutes
#define P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT            5000       //5 seconds
#define P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT       70
#define P2P_DEFAULT_ANCHOR_CONNECTIONS_COUNT            2

#define P2P_FAILED_ADDR_FORGET_SECONDS                  (60*60)     //1 hour
#define P2P_IP_BLOCKTIME_MAINNET                        (60*60*24)  //24 hour
#define P2P_IP_BLOCKTIME_TESTNET                        (60*5)      //5 minutes
#define P2P_IP_FAILS_BEFORE_BLOCK                       10
#define P2P_IDLE_CONNECTION_KILL_INTERVAL               (5*60) //5 minutes
#define P2P_DEFAULT_SOCKS_CONNECT_TIMEOUT               45         // seconds
#define P2P_DEFAULT_SYNC_SEARCH_CONNECTIONS_COUNT       2
#define P2P_DEFAULT_LIMIT_RATE_UP                       2048       // kB/s
#define P2P_DEFAULT_LIMIT_RATE_DOWN                     8192       // kB/s

#define P2P_SUPPORT_FLAG_FLUFFY_BLOCKS                  0x01
#define P2P_SUPPORT_FLAGS                               P2P_SUPPORT_FLAG_FLUFFY_BLOCKS

//#define ALLOW_DEBUG_COMMANDS

#define CRYPTONOTE_NAME                                 "montero"
#define CRYPTONOTE_POOLDATA_FILENAME                    "poolstate.bin"
#define CRYPTONOTE_BLOCKCHAINDATA_FILENAME              "data.mdb"
#define CRYPTONOTE_BLOCKCHAINDATA_LOCK_FILENAME         "lock.mdb"
#define P2P_NET_DATA_FILENAME                           "p2pstate.montero.bin"
#define MINER_CONFIG_FILE_NAME                          "miner_conf.json"

#define THREAD_STACK_SIZE                               5 * 1024 * 1024
#define HASH_OF_HASHES_STEP                             256
#define DEFAULT_TXPOOL_MAX_SIZE                         648000000ull // 3 days at 300000, in bytes

struct hard_fork
{
    uint8_t version;
    uint64_t height;
};

// New constants are intended to go here
namespace config
{
    uint64_t const DEFAULT_DUST_THRESHOLD = 0; // deprecated
    std::string const P2P_REMOTE_DEBUG_TRUSTED_PUB_KEY = "0000000000000000000000000000000000000000000000000000000000000000";

    //Forker: change prefix, net id, port
    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0x01;
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 0x02;
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 0x03;
    uint16_t const P2P_DEFAULT_PORT = 12500;
    uint16_t const RPC_DEFAULT_PORT = 12501;
    uint16_t const ZMQ_RPC_DEFAULT_PORT = 12502;
    boost::uuids::uuid const NETWORK_ID = {{0x43, 0x28, 0xB2, 0x76, 0x62, 0x14, 0x42, 0x22, 0x27, 0x32, 0x21, 0x22, 0x26, 0x21, 0xB2, 0x22}};
    std::string const GENESIS_TX = 
    "010a01ff00010a0227e6b02f99e8eeb539458c9f5d09a1b3a65251ca1cfe4a3c305fefae277bc27221013c63444cd5774695d52714d4d3e03b18edbccbad5187fa541e4d5eb51d1f4af800";

    uint32_t const GENESIS_NONCE = 10000;

    std::string const HF_MIN_VERSION = "1.0.0";
    std::string const MIN_VERSION    = "1.0.0";
    
    std::set<std::string> const seed_nodes = { 
        "51.77.110.246:12500",
        "54.37.130.138:12500"
    };

    static const hard_fork hard_forks[] = {
        {1,1},
        {2,2},
    };

    namespace testnet
    {
        uint16_t const P2P_DEFAULT_PORT = 22500;
        uint16_t const RPC_DEFAULT_PORT = 22501;
        uint16_t const ZMQ_RPC_DEFAULT_PORT = 22502;
        boost::uuids::uuid const NETWORK_ID = {{0x93, 0x42, 0xF0, 0x55, 0x42, 0x18, 0x60, 0x33, 0x16, 0x81, 0x01, 0x92, 0xAA, 0xAC, 0xFF, 0x43}};
		
        std::string const HF_MIN_VERSION = "1.0.0";
        std::string const MIN_VERSION    = "1.0.0";

        std::set<std::string> const seed_nodes = { };

        static const hard_fork hard_forks[] = {
            {1, 1},
            {2, 2},
        };
    }

    namespace stagenet
    {
        uint16_t const P2P_DEFAULT_PORT = 32500;
        uint16_t const RPC_DEFAULT_PORT = 32501;
        uint16_t const ZMQ_RPC_DEFAULT_PORT = 32502;
        boost::uuids::uuid const NETWORK_ID = { {
            0x24, 0x26, 0x11, 0x82 , 0x27, 0x46 , 0x36, 0xFF, 0xBB, 0x51, 0x00, 0x3C, 0x3D, 0xAA, 0x16, 0x2F
        } }; // Bender's daydream

		std::set<std::string> const seed_nodes = { };

        static const hard_fork hard_forks[] = {
            {1, 1},
            {2, 2},
        };
    }
}

#ifndef VERSION_TO_INT
#define VERSION_TO_INT

inline uint32_t version_string_to_integer(std::string data)
{
    const char *v = data.c_str();

    unsigned int byte3;
    unsigned int byte2;
    unsigned int byte1;
    char dummyString[2];

    if (sscanf(v, "%u.%u.%u%1s", &byte3, &byte2, &byte1, dummyString) == 3)
        return (byte3 << 24) + (byte2 << 16) + byte1;

    return 0;
}

#endif

namespace cryptonote
{
    enum network_type : uint8_t
    {
        MAINNET = 0,
        TESTNET,
        STAGENET,
        FAKECHAIN,
        UNDEFINED = 255
    };

    struct config_t
    {
        uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX;
        uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX;
        uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX;
        uint16_t const P2P_DEFAULT_PORT;
        uint16_t const RPC_DEFAULT_PORT;
        uint16_t const ZMQ_RPC_DEFAULT_PORT;
        boost::uuids::uuid const NETWORK_ID;
        std::string const GENESIS_TX;
        uint32_t const GENESIS_NONCE;
        std::string const HF_MIN_VERSION;
        std::string const MIN_VERSION;
    };

    inline const config_t &get_config(network_type nettype)
    {
        static const config_t mainnet = {
            ::config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
            ::config::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
            ::config::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
            ::config::P2P_DEFAULT_PORT,
            ::config::RPC_DEFAULT_PORT,
            ::config::ZMQ_RPC_DEFAULT_PORT,
            ::config::NETWORK_ID,
            ::config::GENESIS_TX,
            ::config::GENESIS_NONCE,
            ::config::HF_MIN_VERSION,
            ::config::MIN_VERSION
            };
        static const config_t testnet = {
            ::config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
            ::config::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
            ::config::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
            ::config::testnet::P2P_DEFAULT_PORT,
            ::config::testnet::RPC_DEFAULT_PORT,
            ::config::testnet::ZMQ_RPC_DEFAULT_PORT,
            ::config::testnet::NETWORK_ID,
            ::config::GENESIS_TX,
            ::config::GENESIS_NONCE,
            ::config::testnet::HF_MIN_VERSION,
            ::config::testnet::MIN_VERSION
            };
        static const config_t stagenet = {
            ::config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
            ::config::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
            ::config::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
            ::config::stagenet::P2P_DEFAULT_PORT,
            ::config::stagenet::RPC_DEFAULT_PORT,
            ::config::stagenet::ZMQ_RPC_DEFAULT_PORT,
            ::config::stagenet::NETWORK_ID,
            ::config::GENESIS_TX,
            ::config::GENESIS_NONCE,
            "", ""
            };

        switch (nettype)
        {
            case MAINNET:
                return mainnet;
            case TESTNET:
                return testnet;
            case STAGENET:
                return stagenet;
            case FAKECHAIN:
                return mainnet;
            default:
                throw std::runtime_error("Invalid network type");
        }
    };
}
