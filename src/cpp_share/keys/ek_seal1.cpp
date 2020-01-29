#include <iostream>
#include <fstream>
#include <vector>

#include "ek_seal.h"
#include "def_seal1.h"

using namespace seal;
using std::cout;
using std::stoull;
using std::string;
using std::vector;

namespace e3
{

bool SealBaseEvalKey::load(string fname)
{
    if (!NOCOUT) cout << "Loading evaluation key .. " << std::flush;

    auto fileParams = fname + ".params.key";
    auto filePublicKey = fname + ".publickey.key";
    auto fileRelin  = fname + ".relin.key";
    auto fileConfig = fname + ".config.key";
    std::ifstream inParams(fileParams, std::ios::binary);
    std::ifstream inPublicKey(filePublicKey, std::ios::binary);
    std::ifstream inRelin (fileRelin , std::ios::binary);
    std::ifstream inConfig(fileConfig, std::ios::binary);
    if ( !inParams || !inPublicKey || !inRelin || !inConfig ) return false;

    static e3seal::SealEvalKey evalkey;
    try
    {
        unsigned char be;
        inConfig.read(reinterpret_cast<char *>(&be), 1);
        evalkey.isBatchEncoder = be == 1;
        static auto params = EncryptionParameters::Load(inParams);
        evalkey.context = SEALContext::Create(params);
        evalkey.publickey.load(evalkey.context, inPublicKey);
        evalkey.relinkeys.load(evalkey.context, inRelin);
        static Evaluator evaluator(evalkey.context);
        static Encryptor encryptor(evalkey.context, evalkey.publickey);
        static IntegerEncoder encoder(evalkey.context);
        if ( evalkey.isBatchEncoder )
        {
            static BatchEncoder batchEncoder(evalkey.context);
            evalkey.batchEncoder = &batchEncoder;
        }
        evalkey.encoder = &encoder;
        evalkey.encryptor = &encryptor;
        evalkey.evaluator = &evaluator;
        evalkey.params = &params;
    }
    catch (...) { throw "Bad " + fname + " eval key"; }

    key = &evalkey;

    if (!NOCOUT) cout << "ok\n";
    return true;
}

string SealBaseEvalKey::rawEncrypt(const string & s, int msz) const
{
    SealNativeCiphertext nb;
    auto evalkey = e3seal::toek(key);
    auto & isBatchEncoder = evalkey->isBatchEncoder;
    auto & encryptor = evalkey->encryptor;
    Plaintext p;
    if ( isBatchEncoder )
    {
        auto & encoder = evalkey->batchEncoder;
        vector<uint64_t> v( encoder->slot_count(), 0LL );
        string tmp = "";
        size_t idx = 0;
        for ( auto c : s )
        {
            if ( c == '_' )
            {
                v[idx++] = stoull(tmp);
                tmp = "";
            }
            else tmp += c;
        }
        uint64_t value = stoull(tmp);
        while ( idx < v.size() ) v[idx++] = value; // repeat the last value to the end
        encoder->encode(v, p);
    }
    else p = evalkey->encoder->encode( (uint64_t) stoull(s) );
    encryptor->encrypt(p, nb.p->ct);
    return nb.str();
}

size_t SealBaseEvalKey::slots()
{
    const auto & k = e3seal::toek(key);
    bool isB = k->isBatchEncoder;
    return size_t(isB ? k->batchEncoder->slot_count() : 1);
}

} // e3
