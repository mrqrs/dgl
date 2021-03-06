/*!
 *  Copyright (c) 2019 by Contributors
 * \file array/cpu/array_op_impl.cc
 * \brief Array operator CPU implementation
 */
#include <dgl/array.h>
#include <numeric>
#include "../arith.h"

namespace dgl {
using runtime::NDArray;
namespace aten {
namespace impl {

///////////////////////////// AsNumBits /////////////////////////////

template <DLDeviceType XPU, typename IdType>
IdArray AsNumBits(IdArray arr, uint8_t bits) {
  CHECK(bits == 32 || bits == 64) << "invalid number of integer bits";
  if (sizeof(IdType) * 8 == bits) {
    return arr;
  }
  const int64_t len = arr->shape[0];
  IdArray ret = NewIdArray(len, arr->ctx, bits);
  const IdType* arr_data = static_cast<IdType*>(arr->data);
  if (bits == 32) {
    int32_t* ret_data = static_cast<int32_t*>(ret->data);
    for (int64_t i = 0; i < len; ++i) {
      ret_data[i] = arr_data[i];
    }
  } else {
    int64_t* ret_data = static_cast<int64_t*>(ret->data);
    for (int64_t i = 0; i < len; ++i) {
      ret_data[i] = arr_data[i];
    }
  }
  return ret;
}

template IdArray AsNumBits<kDLCPU, int32_t>(IdArray arr, uint8_t bits);
template IdArray AsNumBits<kDLCPU, int64_t>(IdArray arr, uint8_t bits);

///////////////////////////// BinaryElewise /////////////////////////////

template <DLDeviceType XPU, typename IdType, typename Op>
IdArray BinaryElewise(IdArray lhs, IdArray rhs) {
  IdArray ret = NewIdArray(lhs->shape[0], lhs->ctx, lhs->dtype.bits);
  const IdType* lhs_data = static_cast<IdType*>(lhs->data);
  const IdType* rhs_data = static_cast<IdType*>(rhs->data);
  IdType* ret_data = static_cast<IdType*>(ret->data);
  // TODO(minjie): we should split the loop into segments for better cache locality.
#pragma omp parallel for
  for (int64_t i = 0; i < lhs->shape[0]; ++i) {
    ret_data[i] = Op::Call(lhs_data[i], rhs_data[i]);
  }
  return ret;
}

template IdArray BinaryElewise<kDLCPU, int32_t, arith::Add>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::Sub>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::Mul>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::Div>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::GT>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::LT>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::GE>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::LE>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::EQ>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::NE>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Add>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Sub>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Mul>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Div>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::GT>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::LT>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::GE>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::LE>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::EQ>(IdArray lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::NE>(IdArray lhs, IdArray rhs);

template <DLDeviceType XPU, typename IdType, typename Op>
IdArray BinaryElewise(IdArray lhs, IdType rhs) {
  IdArray ret = NewIdArray(lhs->shape[0], lhs->ctx, lhs->dtype.bits);
  const IdType* lhs_data = static_cast<IdType*>(lhs->data);
  IdType* ret_data = static_cast<IdType*>(ret->data);
  // TODO(minjie): we should split the loop into segments for better cache locality.
#pragma omp parallel for
  for (int64_t i = 0; i < lhs->shape[0]; ++i) {
    ret_data[i] = Op::Call(lhs_data[i], rhs);
  }
  return ret;
}

template IdArray BinaryElewise<kDLCPU, int32_t, arith::Add>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::Sub>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::Mul>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::Div>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::GT>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::LT>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::GE>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::LE>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::EQ>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::NE>(IdArray lhs, int32_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Add>(IdArray lhs, int64_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Sub>(IdArray lhs, int64_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Mul>(IdArray lhs, int64_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Div>(IdArray lhs, int64_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::GT>(IdArray lhs, int64_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::LT>(IdArray lhs, int64_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::GE>(IdArray lhs, int64_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::LE>(IdArray lhs, int64_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::EQ>(IdArray lhs, int64_t rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::NE>(IdArray lhs, int64_t rhs);

template <DLDeviceType XPU, typename IdType, typename Op>
IdArray BinaryElewise(IdType lhs, IdArray rhs) {
  IdArray ret = NewIdArray(rhs->shape[0], rhs->ctx, rhs->dtype.bits);
  const IdType* rhs_data = static_cast<IdType*>(rhs->data);
  IdType* ret_data = static_cast<IdType*>(ret->data);
  // TODO(minjie): we should split the loop into segments for better cache locality.
#pragma omp parallel for
  for (int64_t i = 0; i < rhs->shape[0]; ++i) {
    ret_data[i] = Op::Call(lhs, rhs_data[i]);
  }
  return ret;
}

template IdArray BinaryElewise<kDLCPU, int32_t, arith::Add>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::Sub>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::Mul>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::Div>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::GT>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::LT>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::GE>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::LE>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::EQ>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int32_t, arith::NE>(int32_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Add>(int64_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Sub>(int64_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Mul>(int64_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::Div>(int64_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::GT>(int64_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::LT>(int64_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::GE>(int64_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::LE>(int64_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::EQ>(int64_t lhs, IdArray rhs);
template IdArray BinaryElewise<kDLCPU, int64_t, arith::NE>(int64_t lhs, IdArray rhs);

template <DLDeviceType XPU, typename IdType, typename Op>
IdArray UnaryElewise(IdArray lhs) {
  IdArray ret = NewIdArray(lhs->shape[0], lhs->ctx, lhs->dtype.bits);
  const IdType* lhs_data = static_cast<IdType*>(lhs->data);
  IdType* ret_data = static_cast<IdType*>(ret->data);
  // TODO(minjie): we should split the loop into segments for better cache locality.
#pragma omp parallel for
  for (int64_t i = 0; i < lhs->shape[0]; ++i) {
    ret_data[i] = Op::Call(lhs_data[i]);
  }
  return ret;
}

template IdArray UnaryElewise<kDLCPU, int32_t, arith::Neg>(IdArray lhs);
template IdArray UnaryElewise<kDLCPU, int64_t, arith::Neg>(IdArray lhs);

///////////////////////////// HStack /////////////////////////////

template <DLDeviceType XPU, typename IdType>
IdArray HStack(IdArray arr1, IdArray arr2) {
  CHECK_EQ(arr1->shape[0], arr2->shape[0]);
  const int64_t L = arr1->shape[0];
  IdArray ret = NewIdArray(2 * L, DLContext{kDLCPU, 0}, arr1->dtype.bits);
  const IdType* arr1_data = static_cast<IdType*>(arr1->data);
  const IdType* arr2_data = static_cast<IdType*>(arr2->data);
  IdType* ret_data = static_cast<IdType*>(ret->data);
  for (int64_t i = 0; i < L; ++i) {
    ret_data[i] = arr1_data[i];
    ret_data[i + L] = arr2_data[i];
  }
  return ret;
}

template IdArray HStack<kDLCPU, int32_t>(IdArray arr1, IdArray arr2);
template IdArray HStack<kDLCPU, int64_t>(IdArray arr1, IdArray arr2);

///////////////////////////// Full /////////////////////////////

template <DLDeviceType XPU, typename IdType>
IdArray Full(IdType val, int64_t length, DLContext ctx) {
  IdArray ret = NewIdArray(length, ctx, sizeof(IdType) * 8);
  IdType* ret_data = static_cast<IdType*>(ret->data);
  std::fill(ret_data, ret_data + length, val);
  return ret;
}

template IdArray Full<kDLCPU, int32_t>(int32_t val, int64_t length, DLContext ctx);
template IdArray Full<kDLCPU, int64_t>(int64_t val, int64_t length, DLContext ctx);

///////////////////////////// Range /////////////////////////////

template <DLDeviceType XPU, typename IdType>
IdArray Range(IdType low, IdType high, DLContext ctx) {
  CHECK(high >= low) << "high must be bigger than low";
  IdArray ret = NewIdArray(high - low, ctx, sizeof(IdType) * 8);
  IdType* ret_data = static_cast<IdType*>(ret->data);
  std::iota(ret_data, ret_data + high - low, low);
  return ret;
}

template IdArray Range<kDLCPU, int32_t>(int32_t, int32_t, DLContext);
template IdArray Range<kDLCPU, int64_t>(int64_t, int64_t, DLContext);

///////////////////////////// Relabel_ /////////////////////////////

template <DLDeviceType XPU, typename IdType>
IdArray Relabel_(const std::vector<IdArray>& arrays) {
  // build map & relabel
  IdType newid = 0;
  std::unordered_map<IdType, IdType> oldv2newv;
  for (IdArray arr : arrays) {
    for (int64_t i = 0; i < arr->shape[0]; ++i) {
      const IdType id = static_cast<IdType*>(arr->data)[i];
      if (!oldv2newv.count(id)) {
        oldv2newv[id] = newid++;
      }
      static_cast<IdType*>(arr->data)[i] = oldv2newv[id];
    }
  }
  // map array
  IdArray maparr = NewIdArray(newid, DLContext{kDLCPU, 0}, sizeof(IdType) * 8);
  IdType* maparr_data = static_cast<IdType*>(maparr->data);
  for (const auto& kv : oldv2newv) {
    maparr_data[kv.second] = kv.first;
  }
  return maparr;
}

template IdArray Relabel_<kDLCPU, int32_t>(const std::vector<IdArray>& arrays);
template IdArray Relabel_<kDLCPU, int64_t>(const std::vector<IdArray>& arrays);


///////////////////////////// NonZero /////////////////////////////

template <DLDeviceType XPU, typename IdType>
IdArray NonZero(BoolArray bool_arr) {
  const IdType* bool_data = static_cast<IdType*>(bool_arr->data);
  CHECK(bool_arr->ndim == 1) << "NonZero only supports 1D array";
  std::vector<IdType> nonzero_indices;
  for (int64_t i = 0; i < bool_arr->shape[0]; i++) {
    if ((bool_data[i]) != 0) {
      nonzero_indices.push_back(i);
    }
  }
  return VecToIdArray(nonzero_indices, sizeof(IdType) * 8);
}

// TODO(Allen): Implement GPU version
template IdArray NonZero<kDLCPU, int32_t>(BoolArray bool_arr);
template IdArray NonZero<kDLCPU, int64_t>(BoolArray bool_arr);

}  // namespace impl
}  // namespace aten
}  // namespace dgl
