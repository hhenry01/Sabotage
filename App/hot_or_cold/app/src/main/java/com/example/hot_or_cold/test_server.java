package com.example.hot_or_cold;

import static com.example.hot_or_cold.constants.MAC_ADDRESS_0;

import androidx.appcompat.app.AppCompatActivity;

import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;

import android.os.Bundle;

import android.view.View;
import android.widget.TextView;

import java.util.List;

//FOR TESTING PURPOSES
public class test_server extends AppCompatActivity {
    private TextView textViewResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test_server);

        textViewResult = findViewById(R.id.server_test);

    }

    public void generate_wifi_info(View view) {
        WifiManager wifi = (WifiManager) getApplicationContext().getSystemService(WIFI_SERVICE);
        wifi.startScan();
        List<ScanResult> scan = wifi.getScanResults();
        String x = "WiFi: \n";
        for(ScanResult wifis : scan) {
            String BSSID = wifis.BSSID;
            String SSID = wifis.SSID;
            int level = wifis.level;
            if(BSSID == null) {
                BSSID = "Could not find BSSID";
            } else if(BSSID.equals(MAC_ADDRESS_0)) {
                x += "\n\nThis is the wifi module mac address:\n";
            }
            if(SSID == null) {
                SSID = "Could not find SSID";
            }
            x += "SSID: " + SSID + "\n";
            x += "BSSID: " + BSSID + "\n";
            x += "Level: " + level + "\n\n";
        }
        textViewResult.setText(x);
    }

}