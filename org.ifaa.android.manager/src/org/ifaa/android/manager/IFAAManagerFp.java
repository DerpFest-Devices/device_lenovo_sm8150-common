package org.ifaa.android.manager;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

import android.compat.annotation.UnsupportedAppUsage;

public class IFAAManagerFp extends IFAAManager {
    private static final String TAG = IFAAManagerFp.class.getSimpleName();
    private static final int VERSION = 1;

    @UnsupportedAppUsage
    public int getVersion() {
        return 1;
    }

    @UnsupportedAppUsage
    public int startBIOManager(Context context, int authType) {
        String str;
        if (context == null || authType != 1) {
            str = TAG;
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.append("context = ");
            stringBuilder.append(context);
            stringBuilder.append(", authType = ");
            stringBuilder.append(authType);
            Log.e(str, stringBuilder.toString());
            return -1;
        }
        str = context.getPackageName();
        String str2 = TAG;
        StringBuilder stringBuilder2 = new StringBuilder();
        stringBuilder2.append("pkgName = ");
        stringBuilder2.append(str);
        stringBuilder2.append(", authType = ");
        stringBuilder2.append(authType);
        Log.d(str2, stringBuilder2.toString());
        context.startActivity(new Intent(IFAAUtils.ACTION_FP_SETTINGS));
        return 0;
    }
}
