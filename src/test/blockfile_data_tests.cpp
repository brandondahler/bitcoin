// Copyright (c) 2012-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "core.h"
#include "main.h"
#include "txdb.h"
#include "util.h"

#include <fstream>
#include <iostream>
#include <map>
#include <stdint.h>

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(blockchain_data_tests)

BOOST_AUTO_TEST_CASE(data_sizess)
{
    if (!pblocktree->LoadBlockIndexGuts())
        cout << "Issue loading guts" << endl;

    //ofstream blockDetails("blockfileData/blockDetails.csv");
    //blockDetails << "File Number,Block Count,File Overhead,Full Size,CBlockHeader,CBlock,Transaction Count" << endl;

    //ofstream transactionDetails("blockfileData/transactionDetails.csv");
    //transactionDetails << "File Number,Block Number,Transaction Number,Transaction Size" << endl;

    //ofstream script33Details("blockfileData/Script33s.csv");
    //script33Details << "File Number,BlockNumber,TransactionNumber,InNumber,Bytes" << endl;
    
    //ofstream script65Details("blockfileData/Script65s.csv");
    //script65Details << "File Number,BlockNumber,TransactionNumber,InNumber,Bytes" << endl;

    //ofstream script73Details("blockfileData/Script73s.csv");
    //script73Details << "File Number,BlockNumber,TransactionNumber,InNumber,Bytes" << endl;

    //ofstream script20Details("blockfileData/Script20s.csv");
    //script20Details << "File Number,Block Number,Transaction Number,Out Number, Bytes" << endl;

    int blockCount = 0;
    uint64_t scriptBytes = 0;
    map<opcodetype, uint64_t> opcodeBytes;
    for (int x = 0; x < 146; ++x)
    {

        CDiskBlockPos pos(x, 0);
        CAutoFile currentFile(OpenBlockFile(pos, true), SER_DISK, CLIENT_VERSION);
        if (!currentFile) 
        {
            cout << "Couldn't open file " << x << endl;
            break;
        }
       
        while (currentFile.good() && !feof(currentFile))
        {
            try
            {
                uint32_t magicNumber;
                uint32_t blockSize;

                CBlock block;
                currentFile >> magicNumber >> blockSize >> block;
                
                //blockDetails 
                //    << x << "," 
                //    << blockCount << ","
                //    << blockSize << ","
                //    << sizeof(magicNumber) + sizeof(blockSize) << ","
                //    << ::GetSerializeSize(*((CBlockHeader*) &block), SER_DISK, CLIENT_VERSION) << ","
                //    << ::GetSerializeSize(block.vtx, SER_DISK, CLIENT_VERSION) << ","
                //    << block.vtx.size()
                //    << endl;


                for (unsigned int y = 0; y < block.vtx.size(); ++y)
                {
                    CTransaction& tx = block.vtx[y];

                    //transactionDetails 
                    //    << x << ","
                    //    << blockCount << ","
                    //    << y << ","
                    //    << ::GetSerializeSize(tx, SER_DISK, CLIENT_VERSION) `
                    //    << endl;
                    
                    for (unsigned int z = 0; z < tx.vin.size(); ++z)
                    {
                        CTxIn& txin = tx.vin[z];

                        scriptBytes += txin.scriptSig.size();
                        opcodetype opcode = (opcodetype) -1;
                        vector<unsigned char> vch;
                        
                        vector<unsigned char>::const_iterator it = txin.scriptSig.begin();
                        while (txin.scriptSig.GetOp(it, opcode, vch))
                        {
                            opcodeBytes[opcode] += 1 + vch.size();
                            
                        //    if (opcode >= 0 && opcode < OP_PUSHDATA1)
                        //    {
                        //        if (vch.size() == 33)
                        //            script33Details
                        //                << x << "," 
                        //                << blockCount << ","
                        //                << y << ","
                        //                << z << ","
                        //                << HexStr(vch) 
                        //                << endl;
                        //        else if (vch.size() == 65)
                        //            script65Details
                        //                << x << "," 
                        //                << blockCount << ","
                        //                << y << ","
                        //                << z << ","
                        //                << HexStr(vch) 
                        //                << endl;
                        //        else if (vch.size() >= 71 && vch.size() <= 73)
                        //            script73Details
                        //                << x << "," 
                        //                << blockCount << ","
                        //                << y << ","
                        //                << z << ","
                        //                << HexStr(vch) 
                        //                << endl;

                        //    }
                        }
                    }
                    
                    
                    for (unsigned int z = 0; z < tx.vout.size(); ++z)
                    {
                        CTxOut& txout = tx.vout[z];

                        scriptBytes += txout.scriptPubKey.size();
                        opcodetype opcode = (opcodetype) -1;
                        vector<unsigned char> vch;

                        vector<unsigned char>::const_iterator it = txout.scriptPubKey.begin();

                        while (txout.scriptPubKey.GetOp(it, opcode, vch))
                        {
                            opcodeBytes[opcode] += 1 + vch.size();
                    //        if (opcode >= 0 && opcode < OP_PUSHDATA1)
                    //        {
                    //            if (vch.size() == 20)
                    //                script20Details
                    //                    << x << ","
                    //                    << blockCount << ","
                    //                    << y << ","
                    //                    << z << ","
                    //                    << HexStr(vch)
                    //                    << endl;
                    //       } 
                        }
                    }
                }


                ++blockCount;

                if (blockCount % 10000 == 0)
                    cout << "File " << x << ": " << blockCount << endl;

            } catch (...) {
            }
        }
    }

    cout << "Total blocks read: " << blockCount << endl;
    cout << "Total script bytes: " << scriptBytes << endl;
    cout << endl;
    cout << "Script opcodes:" << endl;

    for (map<opcodetype, uint64_t>::iterator it = opcodeBytes.begin(); it != opcodeBytes.end(); ++it)
    {
        cout << it->first << " -> " << it->second << endl;
    }
}

BOOST_AUTO_TEST_SUITE_END()
