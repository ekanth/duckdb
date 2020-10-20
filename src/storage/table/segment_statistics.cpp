#include "duckdb/storage/table/segment_statistics.hpp"
#include "duckdb/storage/table/string_statistics.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/planner/table_filter.hpp"

namespace duckdb {

SegmentStatistics::SegmentStatistics(LogicalType type, idx_t type_size) : type(type), type_size(type_size) {
	Reset();
}

SegmentStatistics::SegmentStatistics(LogicalType type, idx_t type_size, unique_ptr<BaseStatistics> stats)
    : type(type), type_size(type_size), statistics(move(stats)) {
}

template<>
void SegmentStatistics::UpdateStatistics<string_t>(const string_t &value) {
	auto &stats = (StringStatistics &) *statistics;
	stats.Update(value);
}

template<>
void SegmentStatistics::UpdateStatistics<interval_t>(const interval_t &value) {
}

void SegmentStatistics::Reset() {
	switch (type.InternalType()) {
	case PhysicalType::BOOL:
	case PhysicalType::INT8:
		statistics = make_unique<NumericStatistics<int8_t>>();
		break;
	case PhysicalType::INT16:
		statistics = make_unique<NumericStatistics<int16_t>>();
		break;
	case PhysicalType::INT32:
		statistics = make_unique<NumericStatistics<int32_t>>();
		break;
	case PhysicalType::INT64:
		statistics = make_unique<NumericStatistics<int64_t>>();
		break;
	case PhysicalType::INT128:
		statistics = make_unique<NumericStatistics<hugeint_t>>();
		break;
	case PhysicalType::FLOAT:
		statistics = make_unique<NumericStatistics<float>>();
		break;
	case PhysicalType::DOUBLE:
		statistics = make_unique<NumericStatistics<double>>();
		break;
	case PhysicalType::VARCHAR:
		statistics = make_unique<StringStatistics>(type.id() == LogicalTypeId::BLOB);
		break;
	case PhysicalType::INTERVAL:
		statistics = make_unique<BaseStatistics>();
		break;
	default:
		throw InternalException("Unimplemented type for SEGMENT statistics");
	}
}

bool SegmentStatistics::CheckZonemap(TableFilter &filter) {
	switch (type.InternalType()) {
	case PhysicalType::INT8: {
		auto constant = filter.constant.value_.tinyint;
		return ((NumericStatistics<int8_t> &) *statistics).CheckZonemap(filter.comparison_type, constant);
	}
	case PhysicalType::INT16: {
		auto constant = filter.constant.value_.smallint;
		return ((NumericStatistics<int16_t> &) *statistics).CheckZonemap(filter.comparison_type, constant);
	}
	case PhysicalType::INT32: {
		auto constant = filter.constant.value_.integer;
		return ((NumericStatistics<int32_t> &) *statistics).CheckZonemap(filter.comparison_type, constant);
	}
	case PhysicalType::INT64: {
		auto constant = filter.constant.value_.bigint;
		return ((NumericStatistics<int64_t> &) *statistics).CheckZonemap(filter.comparison_type, constant);
	}
	case PhysicalType::INT128: {
		auto constant = filter.constant.value_.hugeint;
		return ((NumericStatistics<hugeint_t> &) *statistics).CheckZonemap(filter.comparison_type, constant);
	}
	case PhysicalType::FLOAT: {
		auto constant = filter.constant.value_.float_;
		return ((NumericStatistics<float> &) *statistics).CheckZonemap(filter.comparison_type, constant);
	}
	case PhysicalType::DOUBLE: {
		auto constant = filter.constant.value_.double_;
		return ((NumericStatistics<double> &) *statistics).CheckZonemap(filter.comparison_type, constant);
	}
	case PhysicalType::VARCHAR:
		return ((StringStatistics &) *statistics).CheckZonemap(filter.comparison_type, filter.constant.ToString());
	default:
		return true;
	}
}

}