package com.yeolabgt.mahmoodms.androidssvepinterfacetf;

import java.util.UUID;

/**
 * Created by mahmoodms on 6/30/2016.
 * Constants
 */
public final class AppConstant {
    // To prevent someone from accidentally instantiating the AppConstant class,
    // give it an empty constructor.
    public AppConstant() {
    }

    //Constants
    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";
    /**
     * FOR EMG SIGNAL:
     */
    //Service:
    public static final UUID SERVICE_EMG_SIGNAL = UUID.fromString("228b3260-35fd-875f-39fe-b2a394d28057");
    //Characteristic:
    public static final UUID CHAR_EMG_SIGNAL = UUID.fromString("00003261-0000-1000-8000-00805f9b34fb");
    //Char Data Rate:
    public static final UUID CHAR_EMG_DATARATE = UUID.fromString("00003262-0000-1000-8000-00805f9b34fb");

    /**
     * For new EMG:
     */
    public static final UUID SERVICE_3CH_EMG_SIGNAL = UUID.fromString("228b3460-35fd-875f-39fe-b2a394d28057");
    //Characteristic2:
    public static final UUID CHAR_3CH_EMG_SIGNAL_CH1 = UUID.fromString("00003461-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_3CH_EMG_SIGNAL_CH2 = UUID.fromString("00003462-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_3CH_EMG_SIGNAL_CH3 = UUID.fromString("00003463-0000-1000-8000-00805f9b34fb");
    /**
     * For EOG (new)
     */
    public static final UUID SERVICE_EOG_SIGNAL = UUID.fromString("228be0f0-35fd-875f-39fe-b2a394d28057");//E0F0
    public static final UUID CHAR_EOG_CH1_SIGNAL = UUID.fromString("0000e0f1-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_EOG_CH2_SIGNAL = UUID.fromString("0000e0f2-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_EOG_CH3_SIGNAL = UUID.fromString("0000e0f3-0000-1000-8000-00805f9b34fb");
    /**
     * For EEG:
     */
    public static final UUID SERVICE_EEG_SIGNAL = UUID.fromString("228beef0-35fd-875f-39fe-b2a394d28057");
    public static final UUID SERVICE_EEG_GENERIC = UUID.fromString("0000eef0-0000-1000-8000-00805f9b34fb");
    //Characteristic:
    public static final UUID CHAR_EEG_CH1_SIGNAL = UUID.fromString("0000eef1-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_EEG_CH2_SIGNAL = UUID.fromString("0000eef2-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_EEG_CH3_SIGNAL = UUID.fromString("0000eef3-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_EEG_CH4_SIGNAL = UUID.fromString("0000eef4-0000-1000-8000-00805f9b34fb");
    //Char Data Rate:
    public static final UUID CHAR_EEG_DATARATE = UUID.fromString("0000eef7-0000-1000-8000-00805f9b34fb");
    /**
     * FOR MPU/MOTION SENSOR:
     */
    //Service:
    public static final UUID SERVICE_MPU = UUID.fromString("228ba3a0-35fd-875f-39fe-b2a394d28057");
    //Characteristics:
    public static final UUID CHAR_MPU_ACCEL = UUID.fromString("0000a3a1-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_MPU_GYROS = UUID.fromString("0000a3a2-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_MPU_MAGNT = UUID.fromString("0000a3a3-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_MPU_TEMPR = UUID.fromString("0000a3a4-0000-1000-8000-00805f9b34fb");
    public static final UUID CHAR_MPU_COMBINED = UUID.fromString("0000a3a5-0000-1000-8000-00805f9b34fb");
    // Client Characteristic UUID Values to set for notification.
    // Already declared in BluetoothLe class of BeeLe Library
    public static UUID CLIENT_CHARACTERISTIC_CONFIG = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb");
    // To enable the notification value
    public static final byte[] ENABLE_NOTIFICATION_VALUE = { (byte) 0x01,0x00 };
    // To disable the notification value
    public static final byte[] DISABLE_NOTIFICATION_VALUE = { (byte) 0x00,0x00 };

    /** TO READ THE BLE DEVICE'S INFORMATION **/

    // To read the device information for the device information service.
    public static UUID SERVICE_DEVICE_INFO = UUID.fromString("0000180a-0000-1000-8000-00805f9b34fb");
    // Characteristic UUID to read the each BLE Device's serial number.
    public static UUID CHAR_SERIAL_NUMBER = UUID.fromString("00002a25-0000-1000-8000-00805f9b34fb");
    // Characteristic UUID to read the BLE Device's software version.
    public static UUID CHAR_SOFTWARE_REV = UUID.fromString("00002a28-0000-1000-8000-00805f9b34fb");

    /** TO READ THE BLE DEVICE'S BATTERY LEVEL **/

    // To read the battery information form the Battery information service.
    public static final UUID SERVICE_BATTERY_LEVEL = UUID.fromString("0000180F-0000-1000-8000-00805f9b34fb");
    // Characteristic to read the battery status value.
    public static final UUID CHAR_BATTERY_LEVEL = UUID.fromString("00002a19-0000-1000-8000-00805f9b34fb");

    /** TO DETECT THE BLE DEVICE'S BUTTON PRESS EVENT **/
    // To receive the button press event form the Button information service.
    public static final UUID SERVICE_BUTTON_PRESS_SERVICE = UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb");
    // Characteristic UUID for button press detect event.
    public static final UUID CHAR_BUTTON_PRESS = UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb");

    /**
     * For Ion/NaSensor Signal: (IntraOral pH Sensor)
     */
    public static final UUID SERVICE_ION_NA_SIGNAL = UUID.fromString("228b4260-35fd-875f-39fe-b2a394d28057");
    //Characteristic:
    public static final UUID CHAR_ION_NA_SIGNAL = UUID.fromString("00004261-0000-1000-8000-00805f9b34fb");

    /**
     * For BLE Wheelchair Control
     */
    public static final UUID SERVICE_WHEELCHAIR_CONTROL = UUID.fromString("00009923-1212-efde-1523-785feabcd123");
    public static final UUID CHAR_WHEELCHAIR_BUTTON = UUID.fromString("00009924-1212-efde-1523-785feabcd123");
    public static final UUID CHAR_WHEELCHAIR_CONTROL = UUID.fromString("00009925-1212-efde-1523-785feabcd123");

}
