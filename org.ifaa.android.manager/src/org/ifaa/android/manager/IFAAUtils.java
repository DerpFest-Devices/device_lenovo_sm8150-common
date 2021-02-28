package org.ifaa.android.manager;

import android.compat.annotation.UnsupportedAppUsage;

public class IFAAUtils {
    public static final String ACTION_FP_SETTINGS = "android.settings.FINGERPRINT_SETUP";
    public static final String ACTION_IRIS_SETTINGS = "android.settings.IRIS_SETUP";
    public static final int BIO_TYPE_FINGERPRINT = 1;
    public static final int BIO_TYPE_IRIS = 2;
    public static final int COMMAND_FAIL = -1;
    public static final int COMMAND_OK = 0;

    @UnsupportedAppUsage
    public static boolean authTypeValid(int authType) {
        switch (authType) {
            case BIO_TYPE_FINGERPRINT /*1*/:
            case BIO_TYPE_IRIS /*2*/:
                return true;
            default:
                return false;
        }
    }
}
