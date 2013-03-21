// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: VehicleRecords.proto

#ifndef PROTOBUF_VehicleRecords_2eproto__INCLUDED
#define PROTOBUF_VehicleRecords_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace sap {
namespace nic {
namespace itraffic {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_VehicleRecords_2eproto();
void protobuf_AssignDesc_VehicleRecords_2eproto();
void protobuf_ShutdownFile_VehicleRecords_2eproto();

class Report;
class VehicleReports;

// ===================================================================

class Report : public ::google::protobuf::Message {
 public:
  Report();
  virtual ~Report();

  Report(const Report& from);

  inline Report& operator=(const Report& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Report& default_instance();

  void Swap(Report* other);

  // implements Message ----------------------------------------------

  Report* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Report& from);
  void MergeFrom(const Report& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional fixed64 gpsdata_id = 1;
  inline bool has_gpsdata_id() const;
  inline void clear_gpsdata_id();
  static const int kGpsdataIdFieldNumber = 1;
  inline ::google::protobuf::uint64 gpsdata_id() const;
  inline void set_gpsdata_id(::google::protobuf::uint64 value);

  // required string devid = 2;
  inline bool has_devid() const;
  inline void clear_devid();
  static const int kDevidFieldNumber = 2;
  inline const ::std::string& devid() const;
  inline void set_devid(const ::std::string& value);
  inline void set_devid(const char* value);
  inline void set_devid(const char* value, size_t size);
  inline ::std::string* mutable_devid();
  inline ::std::string* release_devid();
  inline void set_allocated_devid(::std::string* devid);

  // optional sfixed64 stime = 3;
  inline bool has_stime() const;
  inline void clear_stime();
  static const int kStimeFieldNumber = 3;
  inline ::google::protobuf::int64 stime() const;
  inline void set_stime(::google::protobuf::int64 value);

  // optional fixed32 alarmflag = 4;
  inline bool has_alarmflag() const;
  inline void clear_alarmflag();
  static const int kAlarmflagFieldNumber = 4;
  inline ::google::protobuf::uint32 alarmflag() const;
  inline void set_alarmflag(::google::protobuf::uint32 value);

  // optional fixed32 state = 5;
  inline bool has_state() const;
  inline void clear_state();
  static const int kStateFieldNumber = 5;
  inline ::google::protobuf::uint32 state() const;
  inline void set_state(::google::protobuf::uint32 value);

  // optional double latitude = 6;
  inline bool has_latitude() const;
  inline void clear_latitude();
  static const int kLatitudeFieldNumber = 6;
  inline double latitude() const;
  inline void set_latitude(double value);

  // optional double longtitude = 7;
  inline bool has_longtitude() const;
  inline void clear_longtitude();
  static const int kLongtitudeFieldNumber = 7;
  inline double longtitude() const;
  inline void set_longtitude(double value);

  // optional sint32 speed = 8;
  inline bool has_speed() const;
  inline void clear_speed();
  static const int kSpeedFieldNumber = 8;
  inline ::google::protobuf::int32 speed() const;
  inline void set_speed(::google::protobuf::int32 value);

  // optional double orientation = 9;
  inline bool has_orientation() const;
  inline void clear_orientation();
  static const int kOrientationFieldNumber = 9;
  inline double orientation() const;
  inline void set_orientation(double value);

  // required sfixed64 gpstime = 10;
  inline bool has_gpstime() const;
  inline void clear_gpstime();
  static const int kGpstimeFieldNumber = 10;
  inline ::google::protobuf::int64 gpstime() const;
  inline void set_gpstime(::google::protobuf::int64 value);

  // optional double odometer = 11;
  inline bool has_odometer() const;
  inline void clear_odometer();
  static const int kOdometerFieldNumber = 11;
  inline double odometer() const;
  inline void set_odometer(double value);

  // optional double oilgauge = 12;
  inline bool has_oilgauge() const;
  inline void clear_oilgauge();
  static const int kOilgaugeFieldNumber = 12;
  inline double oilgauge() const;
  inline void set_oilgauge(double value);

  // @@protoc_insertion_point(class_scope:com.sap.nic.itraffic.Report)
 private:
  inline void set_has_gpsdata_id();
  inline void clear_has_gpsdata_id();
  inline void set_has_devid();
  inline void clear_has_devid();
  inline void set_has_stime();
  inline void clear_has_stime();
  inline void set_has_alarmflag();
  inline void clear_has_alarmflag();
  inline void set_has_state();
  inline void clear_has_state();
  inline void set_has_latitude();
  inline void clear_has_latitude();
  inline void set_has_longtitude();
  inline void clear_has_longtitude();
  inline void set_has_speed();
  inline void clear_has_speed();
  inline void set_has_orientation();
  inline void clear_has_orientation();
  inline void set_has_gpstime();
  inline void clear_has_gpstime();
  inline void set_has_odometer();
  inline void clear_has_odometer();
  inline void set_has_oilgauge();
  inline void clear_has_oilgauge();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint64 gpsdata_id_;
  ::std::string* devid_;
  ::google::protobuf::int64 stime_;
  ::google::protobuf::uint32 alarmflag_;
  ::google::protobuf::uint32 state_;
  double latitude_;
  double longtitude_;
  double orientation_;
  ::google::protobuf::int64 gpstime_;
  double odometer_;
  double oilgauge_;
  ::google::protobuf::int32 speed_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(12 + 31) / 32];

  friend void  protobuf_AddDesc_VehicleRecords_2eproto();
  friend void protobuf_AssignDesc_VehicleRecords_2eproto();
  friend void protobuf_ShutdownFile_VehicleRecords_2eproto();

  void InitAsDefaultInstance();
  static Report* default_instance_;
};
// -------------------------------------------------------------------

class VehicleReports : public ::google::protobuf::Message {
 public:
  VehicleReports();
  virtual ~VehicleReports();

  VehicleReports(const VehicleReports& from);

  inline VehicleReports& operator=(const VehicleReports& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const VehicleReports& default_instance();

  void Swap(VehicleReports* other);

  // implements Message ----------------------------------------------

  VehicleReports* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const VehicleReports& from);
  void MergeFrom(const VehicleReports& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 report_numer = 1;
  inline bool has_report_numer() const;
  inline void clear_report_numer();
  static const int kReportNumerFieldNumber = 1;
  inline ::google::protobuf::uint32 report_numer() const;
  inline void set_report_numer(::google::protobuf::uint32 value);

  // repeated .com.sap.nic.itraffic.Report report = 2;
  inline int report_size() const;
  inline void clear_report();
  static const int kReportFieldNumber = 2;
  inline const ::com::sap::nic::itraffic::Report& report(int index) const;
  inline ::com::sap::nic::itraffic::Report* mutable_report(int index);
  inline ::com::sap::nic::itraffic::Report* add_report();
  inline const ::google::protobuf::RepeatedPtrField< ::com::sap::nic::itraffic::Report >&
      report() const;
  inline ::google::protobuf::RepeatedPtrField< ::com::sap::nic::itraffic::Report >*
      mutable_report();

  // @@protoc_insertion_point(class_scope:com.sap.nic.itraffic.VehicleReports)
 private:
  inline void set_has_report_numer();
  inline void clear_has_report_numer();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::com::sap::nic::itraffic::Report > report_;
  ::google::protobuf::uint32 report_numer_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_VehicleRecords_2eproto();
  friend void protobuf_AssignDesc_VehicleRecords_2eproto();
  friend void protobuf_ShutdownFile_VehicleRecords_2eproto();

  void InitAsDefaultInstance();
  static VehicleReports* default_instance_;
};
// ===================================================================


// ===================================================================

// Report

// optional fixed64 gpsdata_id = 1;
inline bool Report::has_gpsdata_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Report::set_has_gpsdata_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Report::clear_has_gpsdata_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Report::clear_gpsdata_id() {
  gpsdata_id_ = GOOGLE_ULONGLONG(0);
  clear_has_gpsdata_id();
}
inline ::google::protobuf::uint64 Report::gpsdata_id() const {
  return gpsdata_id_;
}
inline void Report::set_gpsdata_id(::google::protobuf::uint64 value) {
  set_has_gpsdata_id();
  gpsdata_id_ = value;
}

// required string devid = 2;
inline bool Report::has_devid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Report::set_has_devid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Report::clear_has_devid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Report::clear_devid() {
  if (devid_ != &::google::protobuf::internal::kEmptyString) {
    devid_->clear();
  }
  clear_has_devid();
}
inline const ::std::string& Report::devid() const {
  return *devid_;
}
inline void Report::set_devid(const ::std::string& value) {
  set_has_devid();
  if (devid_ == &::google::protobuf::internal::kEmptyString) {
    devid_ = new ::std::string;
  }
  devid_->assign(value);
}
inline void Report::set_devid(const char* value) {
  set_has_devid();
  if (devid_ == &::google::protobuf::internal::kEmptyString) {
    devid_ = new ::std::string;
  }
  devid_->assign(value);
}
inline void Report::set_devid(const char* value, size_t size) {
  set_has_devid();
  if (devid_ == &::google::protobuf::internal::kEmptyString) {
    devid_ = new ::std::string;
  }
  devid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Report::mutable_devid() {
  set_has_devid();
  if (devid_ == &::google::protobuf::internal::kEmptyString) {
    devid_ = new ::std::string;
  }
  return devid_;
}
inline ::std::string* Report::release_devid() {
  clear_has_devid();
  if (devid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = devid_;
    devid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Report::set_allocated_devid(::std::string* devid) {
  if (devid_ != &::google::protobuf::internal::kEmptyString) {
    delete devid_;
  }
  if (devid) {
    set_has_devid();
    devid_ = devid;
  } else {
    clear_has_devid();
    devid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional sfixed64 stime = 3;
inline bool Report::has_stime() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Report::set_has_stime() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Report::clear_has_stime() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Report::clear_stime() {
  stime_ = GOOGLE_LONGLONG(0);
  clear_has_stime();
}
inline ::google::protobuf::int64 Report::stime() const {
  return stime_;
}
inline void Report::set_stime(::google::protobuf::int64 value) {
  set_has_stime();
  stime_ = value;
}

// optional fixed32 alarmflag = 4;
inline bool Report::has_alarmflag() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Report::set_has_alarmflag() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Report::clear_has_alarmflag() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Report::clear_alarmflag() {
  alarmflag_ = 0u;
  clear_has_alarmflag();
}
inline ::google::protobuf::uint32 Report::alarmflag() const {
  return alarmflag_;
}
inline void Report::set_alarmflag(::google::protobuf::uint32 value) {
  set_has_alarmflag();
  alarmflag_ = value;
}

// optional fixed32 state = 5;
inline bool Report::has_state() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Report::set_has_state() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Report::clear_has_state() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Report::clear_state() {
  state_ = 0u;
  clear_has_state();
}
inline ::google::protobuf::uint32 Report::state() const {
  return state_;
}
inline void Report::set_state(::google::protobuf::uint32 value) {
  set_has_state();
  state_ = value;
}

// optional double latitude = 6;
inline bool Report::has_latitude() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void Report::set_has_latitude() {
  _has_bits_[0] |= 0x00000020u;
}
inline void Report::clear_has_latitude() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void Report::clear_latitude() {
  latitude_ = 0;
  clear_has_latitude();
}
inline double Report::latitude() const {
  return latitude_;
}
inline void Report::set_latitude(double value) {
  set_has_latitude();
  latitude_ = value;
}

// optional double longtitude = 7;
inline bool Report::has_longtitude() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void Report::set_has_longtitude() {
  _has_bits_[0] |= 0x00000040u;
}
inline void Report::clear_has_longtitude() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void Report::clear_longtitude() {
  longtitude_ = 0;
  clear_has_longtitude();
}
inline double Report::longtitude() const {
  return longtitude_;
}
inline void Report::set_longtitude(double value) {
  set_has_longtitude();
  longtitude_ = value;
}

// optional sint32 speed = 8;
inline bool Report::has_speed() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void Report::set_has_speed() {
  _has_bits_[0] |= 0x00000080u;
}
inline void Report::clear_has_speed() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void Report::clear_speed() {
  speed_ = 0;
  clear_has_speed();
}
inline ::google::protobuf::int32 Report::speed() const {
  return speed_;
}
inline void Report::set_speed(::google::protobuf::int32 value) {
  set_has_speed();
  speed_ = value;
}

// optional double orientation = 9;
inline bool Report::has_orientation() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void Report::set_has_orientation() {
  _has_bits_[0] |= 0x00000100u;
}
inline void Report::clear_has_orientation() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void Report::clear_orientation() {
  orientation_ = 0;
  clear_has_orientation();
}
inline double Report::orientation() const {
  return orientation_;
}
inline void Report::set_orientation(double value) {
  set_has_orientation();
  orientation_ = value;
}

// required sfixed64 gpstime = 10;
inline bool Report::has_gpstime() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void Report::set_has_gpstime() {
  _has_bits_[0] |= 0x00000200u;
}
inline void Report::clear_has_gpstime() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void Report::clear_gpstime() {
  gpstime_ = GOOGLE_LONGLONG(0);
  clear_has_gpstime();
}
inline ::google::protobuf::int64 Report::gpstime() const {
  return gpstime_;
}
inline void Report::set_gpstime(::google::protobuf::int64 value) {
  set_has_gpstime();
  gpstime_ = value;
}

// optional double odometer = 11;
inline bool Report::has_odometer() const {
  return (_has_bits_[0] & 0x00000400u) != 0;
}
inline void Report::set_has_odometer() {
  _has_bits_[0] |= 0x00000400u;
}
inline void Report::clear_has_odometer() {
  _has_bits_[0] &= ~0x00000400u;
}
inline void Report::clear_odometer() {
  odometer_ = 0;
  clear_has_odometer();
}
inline double Report::odometer() const {
  return odometer_;
}
inline void Report::set_odometer(double value) {
  set_has_odometer();
  odometer_ = value;
}

// optional double oilgauge = 12;
inline bool Report::has_oilgauge() const {
  return (_has_bits_[0] & 0x00000800u) != 0;
}
inline void Report::set_has_oilgauge() {
  _has_bits_[0] |= 0x00000800u;
}
inline void Report::clear_has_oilgauge() {
  _has_bits_[0] &= ~0x00000800u;
}
inline void Report::clear_oilgauge() {
  oilgauge_ = 0;
  clear_has_oilgauge();
}
inline double Report::oilgauge() const {
  return oilgauge_;
}
inline void Report::set_oilgauge(double value) {
  set_has_oilgauge();
  oilgauge_ = value;
}

// -------------------------------------------------------------------

// VehicleReports

// required uint32 report_numer = 1;
inline bool VehicleReports::has_report_numer() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void VehicleReports::set_has_report_numer() {
  _has_bits_[0] |= 0x00000001u;
}
inline void VehicleReports::clear_has_report_numer() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void VehicleReports::clear_report_numer() {
  report_numer_ = 0u;
  clear_has_report_numer();
}
inline ::google::protobuf::uint32 VehicleReports::report_numer() const {
  return report_numer_;
}
inline void VehicleReports::set_report_numer(::google::protobuf::uint32 value) {
  set_has_report_numer();
  report_numer_ = value;
}

// repeated .com.sap.nic.itraffic.Report report = 2;
inline int VehicleReports::report_size() const {
  return report_.size();
}
inline void VehicleReports::clear_report() {
  report_.Clear();
}
inline const ::com::sap::nic::itraffic::Report& VehicleReports::report(int index) const {
  return report_.Get(index);
}
inline ::com::sap::nic::itraffic::Report* VehicleReports::mutable_report(int index) {
  return report_.Mutable(index);
}
inline ::com::sap::nic::itraffic::Report* VehicleReports::add_report() {
  return report_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::sap::nic::itraffic::Report >&
VehicleReports::report() const {
  return report_;
}
inline ::google::protobuf::RepeatedPtrField< ::com::sap::nic::itraffic::Report >*
VehicleReports::mutable_report() {
  return &report_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace itraffic
}  // namespace nic
}  // namespace sap
}  // namespace com

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_VehicleRecords_2eproto__INCLUDED
