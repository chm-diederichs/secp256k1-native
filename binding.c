#include <node_api.h>
#include <string.h>
#include <assert.h>
#include "macros.h"
#include <secp256k1.h>
#include <secp256k1_ecdh.h>
#include <secp256k1_preallocated.h>
#include <secp256k1_recovery.h>
// #include "secp256k1/src/secp256k1.c"

static uint8_t typedarray_width(napi_typedarray_type type) {
  switch (type) {
    case napi_int8_array: return 1;
    case napi_uint8_array: return 1;
    case napi_uint8_clamped_array: return 1;
    case napi_int16_array: return 2;
    case napi_uint16_array: return 2;
    case napi_int32_array: return 4;
    case napi_uint32_array: return 4;
    case napi_float32_array: return 4;
    case napi_float64_array: return 8;
    case napi_bigint64_array: return 8;
    case napi_biguint64_array: return 8;
    default: return 0;
  }
}

napi_value sn_secp256k1_context_preallocated_destroy (napi_env env, void* finalise_data, void* finalise_hint) {
  secp256k1_context_preallocated_destroy(finalise_data);
}

napi_value sn_secp256k1_context_create (napi_env env, napi_callback_info info) {
  SN_ARGV(1, secp256_context_create)

  SN_ARGV_UINT32(flags, 0)

  secp256k1_context *ctx = secp256k1_context_create(flags);

  napi_value buf;

  SN_STATUS_THROWS(napi_create_external_buffer(env, sizeof(ctx), ctx, &sn_secp256k1_context_preallocated_destroy, NULL, &buf), "failed to create a n-api buffer")

  return buf;
}

napi_value sn_secp256k1_ecdsa_signature_serialize_der (napi_env env, napi_callback_info info) {
  SN_ARGV(3, secp256k1_ecdsa_signature_serialize_der)

  SN_ARGV_BUFFER_CAST(secp256k1_context *, ctx, 0)
  SN_ARGV_TYPEDARRAY(output, 1)
  SN_ARGV_BUFFER_CAST(secp256k1_ecdsa_signature *, sig, 2)

  // SN_ASSERT_LENGTH(ctx_size, sizeof(secp256k1_context), ctx)
  SN_THROWS(sig_size != sizeof(secp256k1_ecdsa_signature), "ctx must be 'secp256k1_ecdsa_SIGBYTES' bytes")

  SN_RETURN(secp256k1_ecdsa_signature_serialize_der(ctx, output_data, &output_size, sig), "could not serialise signature")
}

napi_value sn_secp256k1_ecdsa_signature_serialize_compact (napi_env env, napi_callback_info info) {
  SN_ARGV(3, secp256k1_ecdsa_signature_serialize_compact)

  SN_ARGV_BUFFER_CAST(secp256k1_context *, ctx, 0)
  SN_ARGV_TYPEDARRAY(output64, 1)
  SN_ARGV_BUFFER_CAST(secp256k1_ecdsa_signature *, sig, 2)

  // SN_THROWS(ctx_size != sizeof(secp256k1_context), "ctx must be 'secp256k1_CONTEXTBYTES' bytes")
  SN_THROWS(sig_size != sizeof(secp256k1_ecdsa_signature), "sig must be 'secp256k1_ecdsa_SIGBYTES' bytes")
  SN_THROWS(output64_size != 64, "output must be 'secp256k1_ecdsa_COMPACTBYTES'")

  SN_RETURN(secp256k1_ecdsa_signature_serialize_compact(ctx, output64_data, sig), "could not serialise signature")
}

napi_value sn_secp256k1_ecdsa_signature_normalize (napi_env env, napi_callback_info info) {
  SN_ARGV(3, secp256k1_ecdsa_signature_normalize)

  SN_ARGV_BUFFER_CAST(secp256k1_context *, ctx, 0)
  SN_ARGV_BUFFER_CAST(secp256k1_ecdsa_signature *, sigout, 2)
  SN_ARGV_BUFFER_CAST(secp256k1_ecdsa_signature *, sigin, 2)

  // SN_THROWS(ctx_size != sizeof(secp256k1_context), "ctx must be 'secp256k1_CONTEXTBYTES' bytes")
  SN_THROWS(sigout_size != sizeof(secp256k1_ecdsa_signature), "sigout must be 'secp256k1_ecdsa_SIGBYTES' bytes")
  SN_THROWS(sigin_size != sizeof(secp256k1_ecdsa_signature), "sigin must be 'secp256k1_ecdsa_SIGBYTES' bytes")

  SN_RETURN(secp256k1_ecdsa_signature_normalize(ctx, sigout, sigin), "could not serialise signature")
}

napi_value sn_secp256k1_ecdsa_verify (napi_env env, napi_callback_info info) {
  SN_ARGV(4, secp256k1_ecdsa_verify)

  SN_ARGV_BUFFER_CAST(secp256k1_context *, ctx, 0)
  SN_ARGV_BUFFER_CAST(secp256k1_ecdsa_signature *, sig, 1)
  SN_ARGV_TYPEDARRAY(msg32, 2)
  SN_ARGV_BUFFER_CAST(secp256k1_pubkey *, pubkey, 3)

  // SN_THROWS(ctx_size != sizeof(secp256k1_context), "ctx must be 'secp256k1_CONTEXTBYTES' bytes")
  SN_THROWS(sig_size != sizeof(secp256k1_ecdsa_signature), "sig must be 'secp256k1_ecdsa_SIGBYTES' bytes")
  SN_THROWS(msg32_size != 32, "msg32 should be 'secp256k1_ecdsa_MSGBYTES' bytes")
  SN_THROWS(pubkey_size != sizeof(secp256k1_pubkey), "pubkey must be 'secp256k1_PUBKEYBYTES' bytes")

  SN_RETURN_BOOLEAN_FROM_1(secp256k1_ecdsa_verify(ctx, sig, msg32_data, pubkey))
}

// napi_value sn_secp256k1_ecdsa_sign (napi_env env, napi_callback_info info) {
//   SN_ARGV(6, secp256k1_ecdsa_sign)

//   SN_ARGV_BUFFER_CAST(secp256k1_context *, ctx, 0)
//   SN_ARGV_BUFFER_CAST(secp256k1_ecdsa_signature *, signature, 1)
//   SN_ARGV_TYPEDARRAY(msg32, 2)
//   SN_ARGV_TYPEDARRAY(seckey, 3)
//   SN_ARGV_TYPEDARRAY_PTR(nonce_data, 4)
// }

static napi_value create_secp256k1_native(napi_env env) {
  napi_value exports;
  assert(napi_create_object(env, &exports) == napi_ok);

  // SN_EXPORT_UINT32(secp256k1_CONTEXTBYTES, sizeof(secp256k1_context))
  SN_EXPORT_UINT32(secp256k1_PUBKEYBYTES, sizeof(secp256k1_pubkey))
  SN_EXPORT_UINT32(secp256k1_ecdsa_SIGBYTES, sizeof(secp256k1_ecdsa_signature))
  SN_EXPORT_UINT32(secp256k1_ecdsa_COMPACTBYTES, 64)

  SN_EXPORT_FUNCTION(secp256k1_context_preallocated_destroy, sn_secp256k1_context_preallocated_destroy)
  SN_EXPORT_FUNCTION(secp256k1_context_create, sn_secp256k1_context_create)
  SN_EXPORT_FUNCTION(secp256k1_ecdsa_signature_serialize_der, sn_secp256k1_ecdsa_signature_serialize_der)
  SN_EXPORT_FUNCTION(secp256k1_ecdsa_signature_serialize_compact, sn_secp256k1_ecdsa_signature_serialize_compact)
  SN_EXPORT_FUNCTION(secp256k1_ecdsa_signature_normalize, sn_secp256k1_ecdsa_signature_normalize)
  SN_EXPORT_FUNCTION(secp256k1_ecdsa_verify, sn_secp256k1_ecdsa_verify)

  return exports;
}

static napi_value Init(napi_env env, napi_value exports) {
  return create_secp256k1_native(env);
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

