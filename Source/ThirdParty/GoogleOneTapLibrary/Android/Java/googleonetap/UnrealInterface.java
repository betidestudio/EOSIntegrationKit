package com.example.googleonetap;

import android.app.Activity;
import android.content.Intent;
import android.content.IntentSender;
import android.util.Log;

import androidx.annotation.Keep;
import androidx.annotation.NonNull;

import com.google.android.gms.auth.api.identity.BeginSignInRequest;
import com.google.android.gms.auth.api.identity.BeginSignInResult;
import com.google.android.gms.auth.api.identity.Identity;
import com.google.android.gms.auth.api.identity.SignInClient;
import com.google.android.gms.auth.api.identity.SignInCredential;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;

public class UnrealInterface
{
    static private String TAG = "UnrealInterface";
    static private BeginSignInRequest signInRequest;
    static private Activity parentActivity;
    static private String ClientID = "";
    static private SignInClient oneTapClient;
    private static final int REQ_ONE_TAP = 2;

    @Keep
    public static void InitInterface(final Activity lparentActivity, final String client_id)
    {
        Log.d(TAG,"InitInterface"+client_id);
        parentActivity = lparentActivity;
        ClientID = client_id;
    }

    @Keep
    public static void SignIn()
    {
        Log.d(TAG,"SignIn");
        parentActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                oneTapClient = Identity.getSignInClient(parentActivity);
                signInRequest = BeginSignInRequest.builder()
                        .setPasswordRequestOptions(BeginSignInRequest.PasswordRequestOptions.builder()
                                .setSupported(true)
                                .build())
                        .setGoogleIdTokenRequestOptions(BeginSignInRequest.GoogleIdTokenRequestOptions.builder()
                                .setSupported(true)
                                // Your server's client ID, not your Android client ID.
                                .setServerClientId(ClientID)
                                // Only show accounts previously used to sign in.
                                .setFilterByAuthorizedAccounts(false)
                                .build())
                        // Automatically sign in when exactly one credential is retrieved.
                        .setAutoSelectEnabled(true)
                        .build();
                oneTapClient.beginSignIn(signInRequest).addOnSuccessListener(parentActivity, new OnSuccessListener<BeginSignInResult>() {
                            @Override
                            public void onSuccess(BeginSignInResult beginSignInResult) {
                                Log.d(TAG,"onSuccess : "+beginSignInResult.getPendingIntent().getIntentSender().toString());
                                try {
                                    parentActivity.startIntentSenderForResult(
                                            beginSignInResult.getPendingIntent().getIntentSender(), REQ_ONE_TAP,
                                            null, 0, 0, 0);
                                } catch (IntentSender.SendIntentException e) {
                                    e.printStackTrace();
                                    OnGoogleCallback(0, "", "", "", e.toString());
                                }
                            }
                        })
                        .addOnFailureListener(parentActivity, new OnFailureListener() {
                            @Override
                            public void onFailure(@NonNull Exception e) {
                                Log.d(TAG,"onFailure : "+e.toString());
                                OnGoogleCallback(0, "", "", "", e.toString());
                            }
                        });
            }
        });

    }

    @Keep
    public static void OnActivityResult(int requestCode, int resultCode, Intent data)
    {
        if(requestCode == REQ_ONE_TAP)
        {
            try {
                SignInCredential credential = oneTapClient.getSignInCredentialFromIntent(data);
                String idToken = credential.getGoogleIdToken();
                String username = credential.getId();
                String password = credential.getPassword();
                if (idToken !=  null) {
                    Log.d(TAG, "Email : "+idToken);
                    Log.d(TAG, "username : "+username);
                    Log.d(TAG, "password : "+password);
                    OnGoogleCallback(1, username, username, idToken, "");
                }
            } catch (ApiException e) {
                e.printStackTrace();
                OnGoogleCallback(0, "", "", "", e.toString());
            }
        }
    }

    @Keep
    public static native void OnGoogleCallback(int status, String email, String username, String idToken, String error);

}

