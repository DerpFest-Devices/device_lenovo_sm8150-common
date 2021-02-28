package org.ifaa.android.manager;

import android.content.Context;
import android.hardware.fingerprint.FingerprintManager;
import android.os.Build;
import android.os.ServiceManager;
import android.util.Log;
import org.ifaa.android.manager.IIFAAManagerService.Stub;

import android.compat.annotation.UnsupportedAppUsage;

public abstract class IFAAManager {
    String TAG = "IFAA_JAR";

    @UnsupportedAppUsage
    private boolean isFpSupported(Context context) {
        if (context != null) {
            FingerprintManager fpManager = (FingerprintManager) context.getSystemService("fingerprint");
            if (fpManager != null) {
                return fpManager.isHardwareDetected();
            }
        }
        return false;
    }

    @UnsupportedAppUsage
    private boolean isIrisSupported(Context context) {
        return context != null ? false : false;
    }

    @UnsupportedAppUsage
    public String getDeviceModel() {
        if (Build.BRAND.equalsIgnoreCase("motorola")) {
            return getMotoDeviceModel();
        }
        return getLenovoDeviceModel();
    }

    @UnsupportedAppUsage
    private String getLenovoDeviceModel() {
        return Build.MODEL.replace(' ', '-');
    }

    @UnsupportedAppUsage
    private String getMotoDeviceModel() {
        String deviceModel = Build.MODEL.replace('-', '_');
        String brand = Build.BRAND;
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append(brand.substring(0, 1).toUpperCase());
        stringBuilder.append(brand.substring(1));
        brand = stringBuilder.toString();
        stringBuilder = new StringBuilder();
        stringBuilder.append(brand);
        stringBuilder.append("-");
        stringBuilder.append(deviceModel);
        return stringBuilder.toString();
    }

    @UnsupportedAppUsage
    public int getVersion() {
        return 1;
    }

    @UnsupportedAppUsage
    public int getSupportBIOTypes(Context context) {
        int types = 0;
        if (isFpSupported(context)) {
            types = 0 | 1;
        }
        if (isIrisSupported(context)) {
            return types | 2;
        }
        return types;
    }

    @UnsupportedAppUsage
    public int startBIOManager(Context context, int authType) {
        return -1;
    }

    @UnsupportedAppUsage
    public byte[] processCmd(Context context, byte[] param) {
        try {
          IFAAChenUtils mIFAAChenUtils = new IFAAChenUtils();
          return mIFAAChenUtils.processCmd(param);
        } catch (Exception e) {
            Log.e(this.TAG, "FAILED to call mIFAAChenUtils.processCmd");
            e.printStackTrace();
            return null;
        }
    }
}
