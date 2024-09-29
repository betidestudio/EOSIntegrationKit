package com.example.unrealcredentialmanager;

import android.app.Activity;
import android.os.CancellationSignal;
import android.util.Log;

import androidx.annotation.Keep;
import androidx.credentials.Credential;
import androidx.credentials.CredentialManager;
import androidx.credentials.CredentialManagerCallback;
import androidx.credentials.CustomCredential;
import androidx.credentials.GetCredentialRequest;
import androidx.credentials.GetCredentialResponse;
import androidx.credentials.exceptions.GetCredentialException;

import com.google.android.libraries.identity.googleid.GetGoogleIdOption;
import com.google.android.libraries.identity.googleid.GoogleIdTokenCredential;

public class UnrealCredentialManager
{
    static private final String TAG = "UnrealCredentialManager";
    
    private Activity parentActivity;
    private CredentialManager credentialManager;
    
    @Keep
    public void InitCredentialManager(final Activity inParentActivity)
    {
        Log.d(TAG, "Init UE Google Credential Manager");
        
        parentActivity = inParentActivity;
        credentialManager = CredentialManager.create(parentActivity);
    }
    
    @Keep
    public void SignIn(String WebClientID)
    {
        Log.d(TAG, "SignIn Called");
        parentActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        GetGoogleIdOption googleIdOption = new GetGoogleIdOption.Builder()
                                        .setFilterByAuthorizedAccounts(false)
                                        .setServerClientId(WebClientID)
                                        .build();
                        
                        GetCredentialRequest request = new GetCredentialRequest.Builder()
                                        .addCredentialOption(googleIdOption)
                                        .build();
                
                        credentialManager.getCredentialAsync(
                                parentActivity,
                                request,
                                new CancellationSignal(),
                                parentActivity.getMainExecutor(),
                                new CredentialManagerCallback<GetCredentialResponse, GetCredentialException>() {
                                    @Override
                                    public void onResult(GetCredentialResponse result) {
                                        handleSignIn(result);
                                    }
                
                                    @Override
                                    public void onError(GetCredentialException e) {
                                        handleFailure(e);
                                    }
                                }
                        );
                    }
                }
        );
    }
    
    @Keep
    void handleSignIn(GetCredentialResponse response)
    {
        Credential credential = response.getCredential();
        if (credential instanceof CustomCredential) {
            if (GoogleIdTokenCredential.TYPE_GOOGLE_ID_TOKEN_CREDENTIAL.equals(credential.getType())) {
                try {
                    GoogleIdTokenCredential googleIdTokenCredential = GoogleIdTokenCredential.createFrom(((CustomCredential) credential).getData());
                    Log.d(TAG, "Logged In As: " + googleIdTokenCredential.getDisplayName());
                    String googleIDToken = googleIdTokenCredential.getIdToken();
                    String username = googleIdTokenCredential.getDisplayName();
                    OnCredentialManagerCallback(1, username, username, googleIDToken, "");
                    return;
                } catch (Exception e) {
                    Log.e(TAG, "Received an invalid google id token response", e);
                    OnCredentialManagerCallback(0, "", "", "", e.toString());
                    return;
                }
            }
        }
        OnCredentialManagerCallback(0, "", "", "", "Unknown Error while trying to handle SignIn Response!");
    }

    @Keep
    void handleFailure(GetCredentialException e)
    {
        Log.e(TAG, "ERROR:HANDLEFAILURE:"+e.getMessage());
        OnCredentialManagerCallback(0, "", "", "", "CredentialException: "+e.getMessage());
    }
    
    @Keep
    public static native void OnCredentialManagerCallback(int status, String email, String username, String idToken, String error);
}