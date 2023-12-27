// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_userinfo_types.h"

/**
 * The UserInfo Interface is used to receive user information for Epic Account IDs from the backend services and to retrieve that information once it is cached.
 * All UserInfo Interface calls take a handle of type EOS_HUserInfo as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetUserInfoInterface function.
 *
 * @see EOS_Platform_GetUserInfoInterface
 */

/**
 * EOS_UserInfo_QueryUserInfo is used to start an asynchronous query to retrieve information, such as display name, about another account.
 * Once the callback has been fired with a successful ResultCode, it is possible to call EOS_UserInfo_CopyUserInfo to receive an EOS_UserInfo containing the available information.
 *
 * @param Options structure containing the input parameters
 * @param ClientData arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate a callback that is fired when the async operation completes, either successfully or in error
 *
 * @see EOS_UserInfo
 * @see EOS_UserInfo_CopyUserInfo
 * @see EOS_UserInfo_QueryUserInfoOptions
 * @see EOS_UserInfo_OnQueryUserInfoCallback
 */
EOS_DECLARE_FUNC(void) EOS_UserInfo_QueryUserInfo(EOS_HUserInfo Handle, const EOS_UserInfo_QueryUserInfoOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoCallback CompletionDelegate);

/**
 * EOS_UserInfo_QueryUserInfoByDisplayName is used to start an asynchronous query to retrieve user information by display name. This can be useful for getting the EOS_EpicAccountId for a display name.
 * Once the callback has been fired with a successful ResultCode, it is possible to call EOS_UserInfo_CopyUserInfo to receive an EOS_UserInfo containing the available information.
 *
 * @param Options structure containing the input parameters
 * @param ClientData arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate a callback that is fired when the async operation completes, either successfully or in error
 *
 * @see EOS_UserInfo
 * @see EOS_UserInfo_CopyUserInfo
 * @see EOS_UserInfo_QueryUserInfoByDisplayNameOptions
 * @see EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback
 */
EOS_DECLARE_FUNC(void) EOS_UserInfo_QueryUserInfoByDisplayName(EOS_HUserInfo Handle, const EOS_UserInfo_QueryUserInfoByDisplayNameOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback CompletionDelegate);

/**
 * EOS_UserInfo_QueryUserInfoByExternalAccount is used to start an asynchronous query to retrieve user information by external accounts.
 * This can be useful for getting the EOS_EpicAccountId for external accounts.
 * Once the callback has been fired with a successful ResultCode, it is possible to call CopyUserInfo to receive an EOS_UserInfo containing the available information.
 *
 * @param Options structure containing the input parameters
 * @param ClientData arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate a callback that is fired when the async operation completes, either successfully or in error
 *
 * @see EOS_UserInfo
 * @see EOS_UserInfo_QueryUserInfoByExternalAccountOptions
 * @see EOS_UserInfo_OnQueryUserInfoByExternalAccountCallback
 */
EOS_DECLARE_FUNC(void) EOS_UserInfo_QueryUserInfoByExternalAccount(EOS_HUserInfo Handle, const EOS_UserInfo_QueryUserInfoByExternalAccountOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoByExternalAccountCallback CompletionDelegate);

/**
 * EOS_UserInfo_CopyUserInfo is used to immediately retrieve a copy of user information based on an Epic Account ID, cached by a previous call to EOS_UserInfo_QueryUserInfo.
 * If the call returns an EOS_Success result, the out parameter, OutUserInfo, must be passed to EOS_UserInfo_Release to release the memory associated with it.
 *
 * @param Options structure containing the input parameters
 * @param OutUserInfo out parameter used to receive the EOS_UserInfo structure.
 *
 * @return EOS_Success if the information is available and passed out in OutUserInfo
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
 *         EOS_IncompatibleVersion if the API version passed in is incorrect
 *         EOS_NotFound if the user info is not locally cached. The information must have been previously cached by a call to EOS_UserInfo_QueryUserInfo
 *
 * @see EOS_UserInfo
 * @see EOS_UserInfo_CopyUserInfoOptions
 * @see EOS_UserInfo_Release
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UserInfo_CopyUserInfo(EOS_HUserInfo Handle, const EOS_UserInfo_CopyUserInfoOptions* Options, EOS_UserInfo ** OutUserInfo);

/**
 * Fetch the number of external user infos that are cached locally.
 *
 * @param Options The options associated with retrieving the external user info count
 *
 * @see EOS_UserInfo_CopyExternalUserInfoByIndex
 *
 * @return The number of external user infos, or 0 if there is an error
 */
EOS_DECLARE_FUNC(uint32_t) EOS_UserInfo_GetExternalUserInfoCount(EOS_HUserInfo Handle, const EOS_UserInfo_GetExternalUserInfoCountOptions* Options);

/**
 * Fetches an external user info from a given index.
 *
 * @param Options Structure containing the index being accessed
 * @param OutExternalUserInfo The external user info. If it exists and is valid, use EOS_UserInfo_ExternalUserInfo_Release when finished
 *
 * @see EOS_UserInfo_ExternalUserInfo_Release
 *
 * @return EOS_Success if the information is available and passed out in OutExternalUserInfo
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
 *         EOS_NotFound if the external user info is not found
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UserInfo_CopyExternalUserInfoByIndex(EOS_HUserInfo Handle, const EOS_UserInfo_CopyExternalUserInfoByIndexOptions* Options, EOS_UserInfo_ExternalUserInfo ** OutExternalUserInfo);

/**
 * Fetches an external user info for a given external account type.
 *
 * @param Options Structure containing the account type being accessed
 * @param OutExternalUserInfo The external user info. If it exists and is valid, use EOS_UserInfo_ExternalUserInfo_Release when finished
 *
 * @see EOS_UserInfo_ExternalUserInfo_Release
 *
 * @return EOS_Success if the information is available and passed out in OutExternalUserInfo
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
 *         EOS_NotFound if the external user info is not found
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UserInfo_CopyExternalUserInfoByAccountType(EOS_HUserInfo Handle, const EOS_UserInfo_CopyExternalUserInfoByAccountTypeOptions* Options, EOS_UserInfo_ExternalUserInfo ** OutExternalUserInfo);

/**
 * Fetches an external user info for a given external account ID.
 *
 * @param Options Structure containing the account ID being accessed
 * @param OutExternalUserInfo The external user info. If it exists and is valid, use EOS_UserInfo_ExternalUserInfo_Release when finished
 *
 * @see EOS_UserInfo_ExternalUserInfo_Release
 *
 * @return EOS_Success if the information is available and passed out in OutExternalUserInfo
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
 *         EOS_NotFound if the external user info is not found
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UserInfo_CopyExternalUserInfoByAccountId(EOS_HUserInfo Handle, const EOS_UserInfo_CopyExternalUserInfoByAccountIdOptions* Options, EOS_UserInfo_ExternalUserInfo ** OutExternalUserInfo);

/**
 * EOS_UserInfo_CopyBestDisplayName is used to immediately retrieve a copy of user's best display name based on an Epic Account ID.
 * This uses data cached by a previous call to EOS_UserInfo_QueryUserInfo, EOS_UserInfo_QueryUserInfoByDisplayName or EOS_UserInfo_QueryUserInfoByExternalAccount as well as EOS_Connect_QueryExternalAccountMappings.
 * If the call returns an EOS_Success result, the out parameter, OutBestDisplayName, must be passed to EOS_UserInfo_BestDisplayName_Release to release the memory associated with it.
 *
 * @details The current priority for picking display name is as follows:
 * 1. Target is online and friends with user, then use presence platform to determine display name
 * 2. Target is in same lobby or is the owner of a lobby search result, then use lobby platform to determine display name (this requires the target's product user id to be cached)
 * 3. Target is in same rtc room, then use rtc room platform to determine display name (this requires the target's product user id to be cached)
 * @param Options structure containing the input parameters
 * @param OutBestDisplayName out parameter used to receive the EOS_UserInfo_BestDisplayName structure.
 *
 * @return EOS_Success if the information is available and passed out in OutBestDisplayName
 *         EOS_UserInfo_BestDisplayNameIndeterminate unable to determine a cert friendly display name for user, one potential solution would be to call EOS_UserInfo_CopyBestDisplayNameWithPlatform with EOS_OPT_Epic for the platform, see doc for more details
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
 *         EOS_IncompatibleVersion if the API version passed in is incorrect
 *         EOS_NotFound if the user info or product user id is not locally cached
 *
 * @see EOS_UserInfo_QueryUserInfo
 * @see EOS_UserInfo_QueryUserInfoByDisplayName
 * @see EOS_UserInfo_QueryUserInfoByExternalAccount
 * @see EOS_Connect_QueryExternalAccountMappings
 * @see EOS_UserInfo_CopyBestDisplayNameWithPlatform
 * @see EOS_UserInfo_CopyBestDisplayNameOptions
 * @see EOS_UserInfo_BestDisplayName
 * @see EOS_UserInfo_BestDisplayName_Release
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UserInfo_CopyBestDisplayName(EOS_HUserInfo Handle, const EOS_UserInfo_CopyBestDisplayNameOptions* Options, EOS_UserInfo_BestDisplayName ** OutBestDisplayName);

/**
 * EOS_UserInfo_CopyBestDisplayNameWithPlatform is used to immediately retrieve a copy of user's best display name based on an Epic Account ID.
 * This uses data cached by a previous call to EOS_UserInfo_QueryUserInfo, EOS_UserInfo_QueryUserInfoByDisplayName or EOS_UserInfo_QueryUserInfoByExternalAccount.
 * If the call returns an EOS_Success result, the out parameter, OutBestDisplayName, must be passed to EOS_UserInfo_BestDisplayName_Release to release the memory associated with it.
 *
 * @details The current priority for picking display name is as follows:
 * 1. If platform is non-epic, then use platform display name (if the platform is linked to the account)
 * 2. If platform is epic and user has epic display name, then use epic display name
 * 3. If platform is epic and user has no epic display name, then use linked external account display name
 * @param Options structure containing the input parameters
 * @param OutBestDisplayName out parameter used to receive the EOS_UserInfo_BestDisplayName structure.
 *
 * @return EOS_Success if the information is available and passed out in OutBestDisplayName
 *         EOS_UserInfo_BestDisplayNameIndeterminate unable to determine a cert friendly display name for user
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
 *         EOS_IncompatibleVersion if the API version passed in is incorrect
 *         EOS_NotFound if the user info is not locally cached
 *
 * @see EOS_UserInfo_QueryUserInfo
 * @see EOS_UserInfo_QueryUserInfoByDisplayName
 * @see EOS_UserInfo_QueryUserInfoByExternalAccount
 * @see EOS_UserInfo_CopyBestDisplayNameWithPlatformOptions
 * @see EOS_UserInfo_BestDisplayName
 * @see EOS_UserInfo_BestDisplayName_Release
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UserInfo_CopyBestDisplayNameWithPlatform(EOS_HUserInfo Handle, const EOS_UserInfo_CopyBestDisplayNameWithPlatformOptions* Options, EOS_UserInfo_BestDisplayName ** OutBestDisplayName);

/**
 * EOS_UserInfo_GetLocalPlatformType is used to retrieve the online platform type of the current running instance of the game.
 *
 * @param Options structure containing the input parameters
 *
 * @return the online platform type of the current running instance of the game
 *
 * @see EOS_UserInfo_GetLocalPlatformTypeOptions
 */
EOS_DECLARE_FUNC(EOS_OnlinePlatformType) EOS_UserInfo_GetLocalPlatformType(EOS_HUserInfo Handle, const EOS_UserInfo_GetLocalPlatformTypeOptions* Options);
