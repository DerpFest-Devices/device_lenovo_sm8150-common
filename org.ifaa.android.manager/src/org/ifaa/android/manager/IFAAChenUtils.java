package org.ifaa.android.manager;

import android.compat.annotation.UnsupportedAppUsage;
import android.os.IBinder;
import android.os.IHwBinder.DeathRecipient;
import android.util.Slog;

import java.util.ArrayList;

import vendor.zui.hardware.ifaa.V1_0.IIFAADevice;
import vendor.zui.hardware.ifaa.V1_0.IIFAADeviceCallback;

public class IFAAChenUtils {
  String TAG = "IFAAChenUtils";
  private IIFAADevice mDaemon = null;
  public final ArrayList<Byte> list = new ArrayList();
  private DeathRecipient mRecipient = new DeathRecipient() {
      public void serviceDied(long cookie) {
          Slog.e(TAG, "IFAA HAL died");
      }
  };
  private IIFAADeviceCallback mDaemonCallback =
          new IIFAADeviceCallback.Stub() {
      @Override
      public void processCmdCallback(int error, ArrayList<Byte> output) {
          Slog.d("Art_Chen-IFAA", "processCmd, error = " + error + "output = " + output);
          if (error == 0) {
              int n = output.size();
              for (int i = 0; i < n; i++) {
                  list.add(new Byte(((Byte) output.get(i)).byteValue()));
              }
          }
      }
  };
    public byte[] processCmd(byte[] input) {
        IIFAADevice daemon = getDaemon();
        if (daemon != null) {
            try {
                ArrayList<Byte> bytes = new ArrayList();
                int i = 0;
                for (byte valueOf : input) {
                    bytes.add(Byte.valueOf(valueOf));
                }
                Slog.d("Art_Chen-IFAA", "processCmd, bytes = " + bytes);
                daemon.processCmd(bytes, mDaemonCallback);
                int n = list.size();
                byte[] retBuffer = new byte[n];
                while (i < n) {
                    retBuffer[i] = ((Byte) list.get(i)).byteValue();
                    i++;
                }
                return retBuffer;
            } catch (Exception ex) {
                Slog.e(TAG, "Failed to processCmd()", ex);
            }
        }
        return input;
    }

    public synchronized IIFAADevice getDaemon() {
        if (this.mDaemon == null) {
            Slog.d(TAG, "mDaemon was null, reconnect it!");
            try {
                this.mDaemon = IIFAADevice.getService();
            } catch (Exception ex) {
                Slog.e(TAG, "Failed to get IIFAADevice interface", ex);
            }
            if (this.mDaemon == null) {
                Slog.w(TAG, "IFAA HIDL not available!");
                return null;
            }
            if (this.mDaemon != null) {
              this.mDaemon.asBinder().linkToDeath(this.mRecipient, 0);
            }
        }
        return this.mDaemon;
    }
}
