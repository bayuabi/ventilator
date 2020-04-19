package com.example.ubventilator;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.graphics.DashPathEffect;
import android.graphics.drawable.Drawable;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import androidx.core.content.ContextCompat;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.WindowManager;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

import com.github.mikephil.charting.animation.Easing;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.Legend.LegendForm;
import com.github.mikephil.charting.components.LimitLine;
import com.github.mikephil.charting.components.LimitLine.LimitLabelPosition;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.formatter.IFillFormatter;
import com.github.mikephil.charting.highlight.Highlight;
import com.github.mikephil.charting.interfaces.dataprovider.LineDataProvider;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.listener.OnChartValueSelectedListener;
import com.github.mikephil.charting.utils.Utils;

import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.widget.TextView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Toast;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Map;
import java.util.Set;

import android.os.Handler;

import org.json.JSONObject;
import org.json.JSONException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    LineChart mChart;
    private SeekBar seekBarX, seekBarY;
    static final UUID mUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    Thread thread;
    byte buffer[];
    int bufferPosition;
    boolean stopThread;
    private OutputStream outputStream;
    private InputStream inputStream;
    TextView textView;
    private BluetoothSocket btSocket = null;
    JSONObject jsonobj;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

//        textView = (TextView) findViewById(R.id.textbt);

        BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
        System.out.println(btAdapter.getBondedDevices());

        BluetoothDevice hc05 = btAdapter.getRemoteDevice("00:21:13:01:4A:76");
        System.out.println(hc05.getName());

        int counter = 0;
//        do
//        {
        try {
            btSocket = hc05.createRfcommSocketToServiceRecord(mUUID);
            System.out.println(btSocket);
            BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
            btSocket.connect();
            System.out.println(btSocket.isConnected());
        } catch (IOException e) {
            e.printStackTrace();
        }
        counter++;
//        } while (!btSocket.isConnected() && counter < 3);

        mChart = findViewById(R.id.line_chart);
        mChart.getDescription().setEnabled(false);
//        mChart.getDescription().setText("Realtime Pressure Value");
        mChart.getDescription().setTextColor(Color.WHITE);

        mChart.setTouchEnabled(false);
        mChart.setDragEnabled(false);
        mChart.setScaleEnabled(false);
        mChart.setDrawGridBackground(true);
        mChart.setGridBackgroundColor(Color.BLACK);
        mChart.setPinchZoom(false);
        mChart.setBackgroundColor(Color.BLACK);
        LineData data = new LineData();
        data.setValueTextColor(Color.BLUE);
        mChart.setData(data);
//        mChart.getLegend().getCalculatedLineSizes();

        Legend l = mChart.getLegend();
        l.setForm(Legend.LegendForm.LINE);
        l.setFormLineWidth(1f);
        l.setVerticalAlignment(Legend.LegendVerticalAlignment.TOP);
        l.setHorizontalAlignment(Legend.LegendHorizontalAlignment.CENTER);
        l.setOrientation(Legend.LegendOrientation.HORIZONTAL);
        l.setDrawInside(false);
        l.setTextColor(Color.WHITE);
        l.setEnabled(true);
        l.setTextSize(12f);

        XAxis xl = mChart.getXAxis();
        xl.setPosition(XAxis.XAxisPosition.BOTTOM);
        xl.setTextSize(10f);
        xl.setTextColor(Color.RED);
        xl.setDrawAxisLine(true);
        xl.setDrawGridLines(true);
        xl.setDrawLabels(false);
        xl.setEnabled(true);

        YAxis leftAxis = mChart.getAxisLeft();
        leftAxis.setTextColor(Color.WHITE);
        leftAxis.setDrawGridLines(true);
        leftAxis.setGridLineWidth(2f);
        leftAxis.setAxisMinimum(-20f);
        leftAxis.setAxisMaximum(100f);
        leftAxis.setLabelCount(7);
//        leftAxis.setGranularity(7f);
        leftAxis.setEnabled(true);

        YAxis rightAxis = mChart.getAxisRight();
        rightAxis.setLabelCount(7, true);
        rightAxis.setEnabled(true);

        mChart.getAxisLeft().setDrawGridLines(false);
        mChart.getXAxis().setDrawGridLines(false);
        mChart.setDrawBorders(false);





        final Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (!Thread.currentThread().isInterrupted() && !stopThread)
                    while (true) {
                        try {
                            inputStream = btSocket.getInputStream();
                            final int byteCount = inputStream.available();
                            if (byteCount > 0) {
                                String msg = "";
                                int tekanan1 = 0;
                                char ch;
                                while ((ch = (char) inputStream.read()) != '\n') {
                                    msg += ch;
                                }
                                final String message = msg;

                                try {
                                    JSONObject jsonObj = new JSONObject(message);

                                    tekanan1 = jsonObj.getInt("tekanan1");
                                } catch (JSONException e) {
                                    //
                                }
                                final int tekanan = tekanan1;

                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        addEntry((float) tekanan);
                                    }
                                });

//                                Thread.sleep(500);
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }

//                        try{
//                            Thread.sleep(100);
//                        }catch (InterruptedException e){
//                            e.printStackTrace();
//                        }
                    }
            }
        });

        thread.start();

    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onPostResume() {
        super.onPostResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    private LineDataSet createSet() {
        LineDataSet set = new LineDataSet(null, "Pressure (mmHg)");
        set.setAxisDependency(YAxis.AxisDependency.LEFT);
        set.setLineWidth(1f);
        set.setColor(Color.MAGENTA);
//        set.setMode(LineDataSet.Mode.CUBIC_BEZIER);
        set.setCubicIntensity(0.2f);
        set.setDrawCircles(false);
        set.setCircleRadius(0f);
        set.setDrawCircleHole(false);
        set.setDrawValues(false);

        return set;
    }

    private void addEntry(float value) {
        LineData data = mChart.getData();

        if (data != null) {
            ILineDataSet set = data.getDataSetByIndex(0);

            if (set == null) {
                set = createSet();
                data.addDataSet(set);
            }
//            System.out.println(value);

            data.addEntry(new Entry(set.getEntryCount(), value), 0);

            System.out.println(set.getEntryCount());
            data.notifyDataChanged();
            mChart.notifyDataSetChanged();
            mChart.setVisibleXRangeMaximum(500);
            mChart.moveViewToX(data.getEntryCount());

        }


    }
}
