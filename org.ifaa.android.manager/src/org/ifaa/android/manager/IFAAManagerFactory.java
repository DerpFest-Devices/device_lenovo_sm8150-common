package org.ifaa.android.manager;

import android.content.Context;
import android.util.Log;

import android.compat.annotation.UnsupportedAppUsage;

public class IFAAManagerFactory {
    private static final String TAG = IFAAManagerFactory.class.getSimpleName();

    @UnsupportedAppUsage
    public static IFAAManager getIFAAManager(Context context, int authType) {
        if (context == null || !IFAAUtils.authTypeValid(authType)) {
            String str = TAG;
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.append("context = ");
            stringBuilder.append(context);
            stringBuilder.append(", authType = ");
            stringBuilder.append(authType);
            Log.e(str, stringBuilder.toString());
            return null;
        } else if (authType != 1) {
            return null;
        } else {
            return new IFAAManagerFp();
        }
    }
}
