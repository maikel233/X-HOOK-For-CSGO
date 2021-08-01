// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: econ_gcmessages.proto

#ifndef PROTOBUF_econ_5fgcmessages_2eproto__INCLUDED
#define PROTOBUF_econ_5fgcmessages_2eproto__INCLUDED

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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "steammessages.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_econ_5fgcmessages_2eproto();
void protobuf_AssignDesc_econ_5fgcmessages_2eproto();
void protobuf_ShutdownFile_econ_5fgcmessages_2eproto();

class CMsgGCGiftedItems;
class CMsgApplyAutograph;
class CMsgGCUserTrackTimePlayedConsecutively;

enum EGCItemMsg {
  k_EMsgGCBase = 1000,
  k_EMsgGCSetItemPosition = 1001,
  k_EMsgGCCraft = 1002,
  k_EMsgGCCraftResponse = 1003,
  k_EMsgGCDelete = 1004,
  k_EMsgGCVerifyCacheSubscription = 1005,
  k_EMsgGCNameItem = 1006,
  k_EMsgGCUnlockCrate = 1007,
  k_EMsgGCUnlockCrateResponse = 1008,
  k_EMsgGCPaintItem = 1009,
  k_EMsgGCPaintItemResponse = 1010,
  k_EMsgGCGoldenWrenchBroadcast = 1011,
  k_EMsgGCMOTDRequest = 1012,
  k_EMsgGCMOTDRequestResponse = 1013,
  k_EMsgGCAddItemToSocket_DEPRECATED = 1014,
  k_EMsgGCAddItemToSocketResponse_DEPRECATED = 1015,
  k_EMsgGCAddSocketToBaseItem_DEPRECATED = 1016,
  k_EMsgGCAddSocketToItem_DEPRECATED = 1017,
  k_EMsgGCAddSocketToItemResponse_DEPRECATED = 1018,
  k_EMsgGCNameBaseItem = 1019,
  k_EMsgGCNameBaseItemResponse = 1020,
  k_EMsgGCRemoveSocketItem_DEPRECATED = 1021,
  k_EMsgGCRemoveSocketItemResponse_DEPRECATED = 1022,
  k_EMsgGCCustomizeItemTexture = 1023,
  k_EMsgGCCustomizeItemTextureResponse = 1024,
  k_EMsgGCUseItemRequest = 1025,
  k_EMsgGCUseItemResponse = 1026,
  k_EMsgGCGiftedItems_DEPRECATED = 1027,
  k_EMsgGCRemoveItemName = 1030,
  k_EMsgGCRemoveItemPaint = 1031,
  k_EMsgGCGiftWrapItem = 1032,
  k_EMsgGCGiftWrapItemResponse = 1033,
  k_EMsgGCDeliverGift = 1034,
  k_EMsgGCDeliverGiftResponseGiver = 1035,
  k_EMsgGCDeliverGiftResponseReceiver = 1036,
  k_EMsgGCUnwrapGiftRequest = 1037,
  k_EMsgGCUnwrapGiftResponse = 1038,
  k_EMsgGCSetItemStyle = 1039,
  k_EMsgGCUsedClaimCodeItem = 1040,
  k_EMsgGCSortItems = 1041,
  k_EMsgGC_RevolvingLootList_DEPRECATED = 1042,
  k_EMsgGCLookupAccount = 1043,
  k_EMsgGCLookupAccountResponse = 1044,
  k_EMsgGCLookupAccountName = 1045,
  k_EMsgGCLookupAccountNameResponse = 1046,
  k_EMsgGCUpdateItemSchema = 1049,
  k_EMsgGCRemoveCustomTexture = 1051,
  k_EMsgGCRemoveCustomTextureResponse = 1052,
  k_EMsgGCRemoveMakersMark = 1053,
  k_EMsgGCRemoveMakersMarkResponse = 1054,
  k_EMsgGCRemoveUniqueCraftIndex = 1055,
  k_EMsgGCRemoveUniqueCraftIndexResponse = 1056,
  k_EMsgGCSaxxyBroadcast = 1057,
  k_EMsgGCBackpackSortFinished = 1058,
  k_EMsgGCAdjustItemEquippedState = 1059,
  k_EMsgGCCollectItem = 1061,
  k_EMsgGCItemAcknowledged__DEPRECATED = 1062,
  k_EMsgGC_ReportAbuse = 1065,
  k_EMsgGC_ReportAbuseResponse = 1066,
  k_EMsgGCNameItemNotification = 1068,
  k_EMsgGCApplyConsumableEffects = 1069,
  k_EMsgGCConsumableExhausted = 1070,
  k_EMsgGCShowItemsPickedUp = 1071,
  k_EMsgGCClientDisplayNotification = 1072,
  k_EMsgGCApplyStrangePart = 1073,
  k_EMsgGC_IncrementKillCountAttribute = 1074,
  k_EMsgGC_IncrementKillCountResponse = 1075,
  k_EMsgGCApplyPennantUpgrade = 1076,
  k_EMsgGCSetItemPositions = 1077,
  k_EMsgGCApplyEggEssence = 1078,
  k_EMsgGCNameEggEssenceResponse = 1079,
  k_EMsgGCPaintKitItem = 1080,
  k_EMsgGCPaintKitBaseItem = 1081,
  k_EMsgGCPaintKitItemResponse = 1082,
  k_EMsgGCGiftedItems = 1083,
  k_EMsgGCUnlockItemStyle = 1084,
  k_EMsgGCUnlockItemStyleResponse = 1085,
  k_EMsgGCApplySticker = 1086,
  k_EMsgGCItemAcknowledged = 1087,
  k_EMsgGCStatTrakSwap = 1088,
  k_EMsgGCUserTrackTimePlayedConsecutively = 1089,
  k_EMsgGCTradingBase = 1500,
  k_EMsgGCTrading_InitiateTradeRequest = 1501,
  k_EMsgGCTrading_InitiateTradeResponse = 1502,
  k_EMsgGCTrading_StartSession = 1503,
  k_EMsgGCTrading_SetItem = 1504,
  k_EMsgGCTrading_RemoveItem = 1505,
  k_EMsgGCTrading_UpdateTradeInfo = 1506,
  k_EMsgGCTrading_SetReadiness = 1507,
  k_EMsgGCTrading_ReadinessResponse = 1508,
  k_EMsgGCTrading_SessionClosed = 1509,
  k_EMsgGCTrading_CancelSession = 1510,
  k_EMsgGCTrading_TradeChatMsg = 1511,
  k_EMsgGCTrading_ConfirmOffer = 1512,
  k_EMsgGCTrading_TradeTypingChatMsg = 1513,
  k_EMsgGCServerBrowser_FavoriteServer = 1601,
  k_EMsgGCServerBrowser_BlacklistServer = 1602,
  k_EMsgGCServerRentalsBase = 1700,
  k_EMsgGCItemPreviewCheckStatus = 1701,
  k_EMsgGCItemPreviewStatusResponse = 1702,
  k_EMsgGCItemPreviewRequest = 1703,
  k_EMsgGCItemPreviewRequestResponse = 1704,
  k_EMsgGCItemPreviewExpire = 1705,
  k_EMsgGCItemPreviewExpireNotification = 1706,
  k_EMsgGCItemPreviewItemBoughtNotification = 1707,
  k_EMsgGCDev_NewItemRequest = 2001,
  k_EMsgGCDev_NewItemRequestResponse = 2002,
  k_EMsgGCDev_PaintKitDropItem = 2003,
  k_EMsgGCStoreGetUserData = 2500,
  k_EMsgGCStoreGetUserDataResponse = 2501,
  k_EMsgGCStorePurchaseInit_DEPRECATED = 2502,
  k_EMsgGCStorePurchaseInitResponse_DEPRECATED = 2503,
  k_EMsgGCStorePurchaseFinalize = 2504,
  k_EMsgGCStorePurchaseFinalizeResponse = 2505,
  k_EMsgGCStorePurchaseCancel = 2506,
  k_EMsgGCStorePurchaseCancelResponse = 2507,
  k_EMsgGCStorePurchaseQueryTxn = 2508,
  k_EMsgGCStorePurchaseQueryTxnResponse = 2509,
  k_EMsgGCStorePurchaseInit = 2510,
  k_EMsgGCStorePurchaseInitResponse = 2511,
  k_EMsgGCBannedWordListRequest = 2512,
  k_EMsgGCBannedWordListResponse = 2513,
  k_EMsgGCToGCBannedWordListBroadcast = 2514,
  k_EMsgGCToGCBannedWordListUpdated = 2515,
  k_EMsgGCToGCDirtySDOCache = 2516,
  k_EMsgGCToGCDirtyMultipleSDOCache = 2517,
  k_EMsgGCToGCUpdateSQLKeyValue = 2518,
  k_EMsgGCToGCIsTrustedServer = 2519,
  k_EMsgGCToGCIsTrustedServerResponse = 2520,
  k_EMsgGCToGCBroadcastConsoleCommand = 2521,
  k_EMsgGCServerVersionUpdated = 2522,
  k_EMsgGCApplyAutograph = 2523,
  k_EMsgGCToGCWebAPIAccountChanged = 2524,
  k_EMsgGCRequestAnnouncements = 2525,
  k_EMsgGCRequestAnnouncementsResponse = 2526,
  k_EMsgGCRequestPassportItemGrant = 2527,
  k_EMsgGCClientVersionUpdated = 2528
};
bool EGCItemMsg_IsValid(int value);
const EGCItemMsg EGCItemMsg_MIN = k_EMsgGCBase;
const EGCItemMsg EGCItemMsg_MAX = k_EMsgGCClientVersionUpdated;
const int EGCItemMsg_ARRAYSIZE = EGCItemMsg_MAX + 1;

const ::google::protobuf::EnumDescriptor* EGCItemMsg_descriptor();
inline const ::std::string& EGCItemMsg_Name(EGCItemMsg value) {
  return ::google::protobuf::internal::NameOfEnum(
    EGCItemMsg_descriptor(), value);
}
inline bool EGCItemMsg_Parse(
    const ::std::string& name, EGCItemMsg* value) {
  return ::google::protobuf::internal::ParseNamedEnum<EGCItemMsg>(
    EGCItemMsg_descriptor(), name, value);
}
enum EGCMsgResponse {
  k_EGCMsgResponseOK = 0,
  k_EGCMsgResponseDenied = 1,
  k_EGCMsgResponseServerError = 2,
  k_EGCMsgResponseTimeout = 3,
  k_EGCMsgResponseInvalid = 4,
  k_EGCMsgResponseNoMatch = 5,
  k_EGCMsgResponseUnknownError = 6,
  k_EGCMsgResponseNotLoggedOn = 7,
  k_EGCMsgFailedToCreate = 8,
  k_EGCMsgLimitExceeded = 9,
  k_EGCMsgCommitUnfinalized = 10
};
bool EGCMsgResponse_IsValid(int value);
const EGCMsgResponse EGCMsgResponse_MIN = k_EGCMsgResponseOK;
const EGCMsgResponse EGCMsgResponse_MAX = k_EGCMsgCommitUnfinalized;
const int EGCMsgResponse_ARRAYSIZE = EGCMsgResponse_MAX + 1;

const ::google::protobuf::EnumDescriptor* EGCMsgResponse_descriptor();
inline const ::std::string& EGCMsgResponse_Name(EGCMsgResponse value) {
  return ::google::protobuf::internal::NameOfEnum(
    EGCMsgResponse_descriptor(), value);
}
inline bool EGCMsgResponse_Parse(
    const ::std::string& name, EGCMsgResponse* value) {
  return ::google::protobuf::internal::ParseNamedEnum<EGCMsgResponse>(
    EGCMsgResponse_descriptor(), name, value);
}
enum EUnlockStyle {
  k_UnlockStyle_Succeeded = 0,
  k_UnlockStyle_Failed_PreReq = 1,
  k_UnlockStyle_Failed_CantAfford = 2,
  k_UnlockStyle_Failed_CantCommit = 3,
  k_UnlockStyle_Failed_CantLockCache = 4,
  k_UnlockStyle_Failed_CantAffordAttrib = 5
};
bool EUnlockStyle_IsValid(int value);
const EUnlockStyle EUnlockStyle_MIN = k_UnlockStyle_Succeeded;
const EUnlockStyle EUnlockStyle_MAX = k_UnlockStyle_Failed_CantAffordAttrib;
const int EUnlockStyle_ARRAYSIZE = EUnlockStyle_MAX + 1;

const ::google::protobuf::EnumDescriptor* EUnlockStyle_descriptor();
inline const ::std::string& EUnlockStyle_Name(EUnlockStyle value) {
  return ::google::protobuf::internal::NameOfEnum(
    EUnlockStyle_descriptor(), value);
}
inline bool EUnlockStyle_Parse(
    const ::std::string& name, EUnlockStyle* value) {
  return ::google::protobuf::internal::ParseNamedEnum<EUnlockStyle>(
    EUnlockStyle_descriptor(), name, value);
}
// ===================================================================

class CMsgGCGiftedItems : public ::google::protobuf::Message {
 public:
  CMsgGCGiftedItems();
  virtual ~CMsgGCGiftedItems();

  CMsgGCGiftedItems(const CMsgGCGiftedItems& from);

  inline CMsgGCGiftedItems& operator=(const CMsgGCGiftedItems& from) {
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
  static const CMsgGCGiftedItems& default_instance();

  void Swap(CMsgGCGiftedItems* other);

  // implements Message ----------------------------------------------

  CMsgGCGiftedItems* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMsgGCGiftedItems& from);
  void MergeFrom(const CMsgGCGiftedItems& from);
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

  // optional uint32 accountid = 1;
  inline bool has_accountid() const;
  inline void clear_accountid();
  static const int kAccountidFieldNumber = 1;
  inline ::google::protobuf::uint32 accountid() const;
  inline void set_accountid(::google::protobuf::uint32 value);

  // optional uint32 giftdefindex = 2;
  inline bool has_giftdefindex() const;
  inline void clear_giftdefindex();
  static const int kGiftdefindexFieldNumber = 2;
  inline ::google::protobuf::uint32 giftdefindex() const;
  inline void set_giftdefindex(::google::protobuf::uint32 value);

  // optional uint32 max_gifts_possible = 3;
  inline bool has_max_gifts_possible() const;
  inline void clear_max_gifts_possible();
  static const int kMaxGiftsPossibleFieldNumber = 3;
  inline ::google::protobuf::uint32 max_gifts_possible() const;
  inline void set_max_gifts_possible(::google::protobuf::uint32 value);

  // optional uint32 num_eligible_recipients = 4;
  inline bool has_num_eligible_recipients() const;
  inline void clear_num_eligible_recipients();
  static const int kNumEligibleRecipientsFieldNumber = 4;
  inline ::google::protobuf::uint32 num_eligible_recipients() const;
  inline void set_num_eligible_recipients(::google::protobuf::uint32 value);

  // repeated uint32 recipients_accountids = 5;
  inline int recipients_accountids_size() const;
  inline void clear_recipients_accountids();
  static const int kRecipientsAccountidsFieldNumber = 5;
  inline ::google::protobuf::uint32 recipients_accountids(int index) const;
  inline void set_recipients_accountids(int index, ::google::protobuf::uint32 value);
  inline void add_recipients_accountids(::google::protobuf::uint32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      recipients_accountids() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_recipients_accountids();

  // @@protoc_insertion_point(class_scope:CMsgGCGiftedItems)
 private:
  inline void set_has_accountid();
  inline void clear_has_accountid();
  inline void set_has_giftdefindex();
  inline void clear_has_giftdefindex();
  inline void set_has_max_gifts_possible();
  inline void clear_has_max_gifts_possible();
  inline void set_has_num_eligible_recipients();
  inline void clear_has_num_eligible_recipients();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 accountid_;
  ::google::protobuf::uint32 giftdefindex_;
  ::google::protobuf::uint32 max_gifts_possible_;
  ::google::protobuf::uint32 num_eligible_recipients_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > recipients_accountids_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_econ_5fgcmessages_2eproto();
  friend void protobuf_AssignDesc_econ_5fgcmessages_2eproto();
  friend void protobuf_ShutdownFile_econ_5fgcmessages_2eproto();

  void InitAsDefaultInstance();
  static CMsgGCGiftedItems* default_instance_;
};
// -------------------------------------------------------------------

class CMsgApplyAutograph : public ::google::protobuf::Message {
 public:
  CMsgApplyAutograph();
  virtual ~CMsgApplyAutograph();

  CMsgApplyAutograph(const CMsgApplyAutograph& from);

  inline CMsgApplyAutograph& operator=(const CMsgApplyAutograph& from) {
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
  static const CMsgApplyAutograph& default_instance();

  void Swap(CMsgApplyAutograph* other);

  // implements Message ----------------------------------------------

  CMsgApplyAutograph* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMsgApplyAutograph& from);
  void MergeFrom(const CMsgApplyAutograph& from);
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

  // optional uint64 autograph_item_id = 1;
  inline bool has_autograph_item_id() const;
  inline void clear_autograph_item_id();
  static const int kAutographItemIdFieldNumber = 1;
  inline ::google::protobuf::uint64 autograph_item_id() const;
  inline void set_autograph_item_id(::google::protobuf::uint64 value);

  // optional uint64 item_item_id = 2;
  inline bool has_item_item_id() const;
  inline void clear_item_item_id();
  static const int kItemItemIdFieldNumber = 2;
  inline ::google::protobuf::uint64 item_item_id() const;
  inline void set_item_item_id(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:CMsgApplyAutograph)
 private:
  inline void set_has_autograph_item_id();
  inline void clear_has_autograph_item_id();
  inline void set_has_item_item_id();
  inline void clear_has_item_item_id();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint64 autograph_item_id_;
  ::google::protobuf::uint64 item_item_id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_econ_5fgcmessages_2eproto();
  friend void protobuf_AssignDesc_econ_5fgcmessages_2eproto();
  friend void protobuf_ShutdownFile_econ_5fgcmessages_2eproto();

  void InitAsDefaultInstance();
  static CMsgApplyAutograph* default_instance_;
};
// -------------------------------------------------------------------

class CMsgGCUserTrackTimePlayedConsecutively : public ::google::protobuf::Message {
 public:
  CMsgGCUserTrackTimePlayedConsecutively();
  virtual ~CMsgGCUserTrackTimePlayedConsecutively();

  CMsgGCUserTrackTimePlayedConsecutively(const CMsgGCUserTrackTimePlayedConsecutively& from);

  inline CMsgGCUserTrackTimePlayedConsecutively& operator=(const CMsgGCUserTrackTimePlayedConsecutively& from) {
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
  static const CMsgGCUserTrackTimePlayedConsecutively& default_instance();

  void Swap(CMsgGCUserTrackTimePlayedConsecutively* other);

  // implements Message ----------------------------------------------

  CMsgGCUserTrackTimePlayedConsecutively* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMsgGCUserTrackTimePlayedConsecutively& from);
  void MergeFrom(const CMsgGCUserTrackTimePlayedConsecutively& from);
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

  // optional uint32 state = 1;
  inline bool has_state() const;
  inline void clear_state();
  static const int kStateFieldNumber = 1;
  inline ::google::protobuf::uint32 state() const;
  inline void set_state(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMsgGCUserTrackTimePlayedConsecutively)
 private:
  inline void set_has_state();
  inline void clear_has_state();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 state_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_econ_5fgcmessages_2eproto();
  friend void protobuf_AssignDesc_econ_5fgcmessages_2eproto();
  friend void protobuf_ShutdownFile_econ_5fgcmessages_2eproto();

  void InitAsDefaultInstance();
  static CMsgGCUserTrackTimePlayedConsecutively* default_instance_;
};
// ===================================================================


// ===================================================================

// CMsgGCGiftedItems

// optional uint32 accountid = 1;
inline bool CMsgGCGiftedItems::has_accountid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CMsgGCGiftedItems::set_has_accountid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CMsgGCGiftedItems::clear_has_accountid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CMsgGCGiftedItems::clear_accountid() {
  accountid_ = 0u;
  clear_has_accountid();
}
inline ::google::protobuf::uint32 CMsgGCGiftedItems::accountid() const {
  return accountid_;
}
inline void CMsgGCGiftedItems::set_accountid(::google::protobuf::uint32 value) {
  set_has_accountid();
  accountid_ = value;
}

// optional uint32 giftdefindex = 2;
inline bool CMsgGCGiftedItems::has_giftdefindex() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CMsgGCGiftedItems::set_has_giftdefindex() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CMsgGCGiftedItems::clear_has_giftdefindex() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CMsgGCGiftedItems::clear_giftdefindex() {
  giftdefindex_ = 0u;
  clear_has_giftdefindex();
}
inline ::google::protobuf::uint32 CMsgGCGiftedItems::giftdefindex() const {
  return giftdefindex_;
}
inline void CMsgGCGiftedItems::set_giftdefindex(::google::protobuf::uint32 value) {
  set_has_giftdefindex();
  giftdefindex_ = value;
}

// optional uint32 max_gifts_possible = 3;
inline bool CMsgGCGiftedItems::has_max_gifts_possible() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void CMsgGCGiftedItems::set_has_max_gifts_possible() {
  _has_bits_[0] |= 0x00000004u;
}
inline void CMsgGCGiftedItems::clear_has_max_gifts_possible() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void CMsgGCGiftedItems::clear_max_gifts_possible() {
  max_gifts_possible_ = 0u;
  clear_has_max_gifts_possible();
}
inline ::google::protobuf::uint32 CMsgGCGiftedItems::max_gifts_possible() const {
  return max_gifts_possible_;
}
inline void CMsgGCGiftedItems::set_max_gifts_possible(::google::protobuf::uint32 value) {
  set_has_max_gifts_possible();
  max_gifts_possible_ = value;
}

// optional uint32 num_eligible_recipients = 4;
inline bool CMsgGCGiftedItems::has_num_eligible_recipients() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void CMsgGCGiftedItems::set_has_num_eligible_recipients() {
  _has_bits_[0] |= 0x00000008u;
}
inline void CMsgGCGiftedItems::clear_has_num_eligible_recipients() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void CMsgGCGiftedItems::clear_num_eligible_recipients() {
  num_eligible_recipients_ = 0u;
  clear_has_num_eligible_recipients();
}
inline ::google::protobuf::uint32 CMsgGCGiftedItems::num_eligible_recipients() const {
  return num_eligible_recipients_;
}
inline void CMsgGCGiftedItems::set_num_eligible_recipients(::google::protobuf::uint32 value) {
  set_has_num_eligible_recipients();
  num_eligible_recipients_ = value;
}

// repeated uint32 recipients_accountids = 5;
inline int CMsgGCGiftedItems::recipients_accountids_size() const {
  return recipients_accountids_.size();
}
inline void CMsgGCGiftedItems::clear_recipients_accountids() {
  recipients_accountids_.Clear();
}
inline ::google::protobuf::uint32 CMsgGCGiftedItems::recipients_accountids(int index) const {
  return recipients_accountids_.Get(index);
}
inline void CMsgGCGiftedItems::set_recipients_accountids(int index, ::google::protobuf::uint32 value) {
  recipients_accountids_.Set(index, value);
}
inline void CMsgGCGiftedItems::add_recipients_accountids(::google::protobuf::uint32 value) {
  recipients_accountids_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
CMsgGCGiftedItems::recipients_accountids() const {
  return recipients_accountids_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
CMsgGCGiftedItems::mutable_recipients_accountids() {
  return &recipients_accountids_;
}

// -------------------------------------------------------------------

// CMsgApplyAutograph

// optional uint64 autograph_item_id = 1;
inline bool CMsgApplyAutograph::has_autograph_item_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CMsgApplyAutograph::set_has_autograph_item_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CMsgApplyAutograph::clear_has_autograph_item_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CMsgApplyAutograph::clear_autograph_item_id() {
  autograph_item_id_ = GOOGLE_ULONGLONG(0);
  clear_has_autograph_item_id();
}
inline ::google::protobuf::uint64 CMsgApplyAutograph::autograph_item_id() const {
  return autograph_item_id_;
}
inline void CMsgApplyAutograph::set_autograph_item_id(::google::protobuf::uint64 value) {
  set_has_autograph_item_id();
  autograph_item_id_ = value;
}

// optional uint64 item_item_id = 2;
inline bool CMsgApplyAutograph::has_item_item_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CMsgApplyAutograph::set_has_item_item_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CMsgApplyAutograph::clear_has_item_item_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CMsgApplyAutograph::clear_item_item_id() {
  item_item_id_ = GOOGLE_ULONGLONG(0);
  clear_has_item_item_id();
}
inline ::google::protobuf::uint64 CMsgApplyAutograph::item_item_id() const {
  return item_item_id_;
}
inline void CMsgApplyAutograph::set_item_item_id(::google::protobuf::uint64 value) {
  set_has_item_item_id();
  item_item_id_ = value;
}

// -------------------------------------------------------------------

// CMsgGCUserTrackTimePlayedConsecutively

// optional uint32 state = 1;
inline bool CMsgGCUserTrackTimePlayedConsecutively::has_state() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CMsgGCUserTrackTimePlayedConsecutively::set_has_state() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CMsgGCUserTrackTimePlayedConsecutively::clear_has_state() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CMsgGCUserTrackTimePlayedConsecutively::clear_state() {
  state_ = 0u;
  clear_has_state();
}
inline ::google::protobuf::uint32 CMsgGCUserTrackTimePlayedConsecutively::state() const {
  return state_;
}
inline void CMsgGCUserTrackTimePlayedConsecutively::set_state(::google::protobuf::uint32 value) {
  set_has_state();
  state_ = value;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::EGCItemMsg>() {
  return ::EGCItemMsg_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::EGCMsgResponse>() {
  return ::EGCMsgResponse_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::EUnlockStyle>() {
  return ::EUnlockStyle_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_econ_5fgcmessages_2eproto__INCLUDED