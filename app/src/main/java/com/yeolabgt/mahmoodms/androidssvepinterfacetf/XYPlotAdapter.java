package com.yeolabgt.mahmoodms.androidssvepinterfacetf;

import android.graphics.Color;
import android.view.View;

import com.androidplot.xy.BoundaryMode;
import com.androidplot.xy.XYPlot;
import com.androidplot.xy.XYStepMode;

import java.text.DecimalFormat;

/**
 * Created by mahmoodms on 5/15/2017.
 *
 */

class XYPlotAdapter {

//    private final static String TAG = XYPlotAdapter.class.getSimpleName();
    XYPlot xyPlot = null;

    /**
     * This function implies that plotImplicitXVals is false. Therefore domain parameters need to be specified:
     * @param findViewByID R.id in /res/
     * @param domainLabel x-axis label
     * @param rangeLabel y-axis label
     * @param domainIncrement x-axis increment
     */
    XYPlotAdapter(View findViewByID, String domainLabel, String rangeLabel,  double domainIncrement) {
        this.xyPlot = (XYPlot) findViewByID;
        this.xyPlot.setDomainBoundaries(0, 1, BoundaryMode.AUTO); //Default
        this.xyPlot.setDomainStepMode(XYStepMode.INCREMENT_BY_VAL);
        this.xyPlot.setDomainStepValue(domainIncrement);
        //Default Config:
        this.xyPlot.setRangeStepMode(XYStepMode.INCREMENT_BY_VAL);
        this.xyPlot.setDomainLabel(domainLabel);
        this.xyPlot.getDomainLabelWidget().pack();
        this.xyPlot.setRangeLabel(rangeLabel);
        this.xyPlot.getRangeLabelWidget().pack();
        this.xyPlot.setRangeValueFormat(new DecimalFormat("#.###"));
        this.xyPlot.setDomainValueFormat(new DecimalFormat("#"));
        this.xyPlot.getDomainLabelWidget().getLabelPaint().setColor(Color.BLACK);
        this.xyPlot.getDomainLabelWidget().getLabelPaint().setTextSize(20);
        this.xyPlot.getRangeLabelWidget().getLabelPaint().setColor(Color.BLACK);
        this.xyPlot.getRangeLabelWidget().getLabelPaint().setTextSize(20);
        this.xyPlot.getGraphWidget().getDomainTickLabelPaint().setColor(Color.BLACK);
        this.xyPlot.getGraphWidget().getRangeTickLabelPaint().setColor(Color.BLACK);
        this.xyPlot.getGraphWidget().getDomainTickLabelPaint().setTextSize(23); //was 36
        this.xyPlot.getGraphWidget().getRangeTickLabelPaint().setTextSize(23);
        this.xyPlot.getGraphWidget().getDomainGridLinePaint().setColor(Color.WHITE);
        this.xyPlot.getGraphWidget().getRangeGridLinePaint().setColor(Color.WHITE);
        this.xyPlot.getLegendWidget().getTextPaint().setColor(Color.BLACK);
        this.xyPlot.getLegendWidget().getTextPaint().setTextSize(20);
        this.xyPlot.getTitleWidget().getLabelPaint().setTextSize(20);
        this.xyPlot.getTitleWidget().getLabelPaint().setColor(Color.BLACK);
        this.xyPlot.setRangeBoundaries(-0.004, 0.004, BoundaryMode.AUTO);
        this.xyPlot.setRangeStep(XYStepMode.SUBDIVIDE, 5.0);
    }

    XYPlotAdapter(View findViewByID, boolean plotImplicitXVals, int historySize) {
        this.xyPlot = (XYPlot) findViewByID;
        int historySeconds = historySize/250;
        if(plotImplicitXVals) {
            this.xyPlot.setDomainBoundaries(0, historySize, BoundaryMode.FIXED);
            this.xyPlot.setDomainStepMode(XYStepMode.INCREMENT_BY_VAL);
            this.xyPlot.setDomainStepValue(historySize/5);
        } else {
            this.xyPlot.setDomainBoundaries(0, historySeconds, BoundaryMode.AUTO);
            this.xyPlot.setDomainStepMode(XYStepMode.INCREMENT_BY_VAL);
            this.xyPlot.setDomainStepValue(historySeconds / 4);
        }
        //Default Config:
        this.xyPlot.setRangeStepMode(XYStepMode.INCREMENT_BY_VAL);
        this.xyPlot.setDomainLabel("Time (seconds)");
        this.xyPlot.getDomainLabelWidget().pack();
        this.xyPlot.setRangeLabel("Voltage (mV)");
        this.xyPlot.getRangeLabelWidget().pack();
        this.xyPlot.setRangeValueFormat(new DecimalFormat("#.###"));
        this.xyPlot.setDomainValueFormat(new DecimalFormat("#"));
        this.xyPlot.getDomainLabelWidget().getLabelPaint().setColor(Color.BLACK);
        this.xyPlot.getDomainLabelWidget().getLabelPaint().setTextSize(20);
        this.xyPlot.getRangeLabelWidget().getLabelPaint().setColor(Color.BLACK);
        this.xyPlot.getRangeLabelWidget().getLabelPaint().setTextSize(20);
        this.xyPlot.getGraphWidget().getDomainTickLabelPaint().setColor(Color.BLACK);
        this.xyPlot.getGraphWidget().getRangeTickLabelPaint().setColor(Color.BLACK);
        this.xyPlot.getGraphWidget().getDomainTickLabelPaint().setTextSize(23); //was 36
        this.xyPlot.getGraphWidget().getRangeTickLabelPaint().setTextSize(23);
        this.xyPlot.getGraphWidget().getDomainGridLinePaint().setColor(Color.WHITE);
        this.xyPlot.getGraphWidget().getRangeGridLinePaint().setColor(Color.WHITE);
        this.xyPlot.getLegendWidget().getTextPaint().setColor(Color.BLACK);
        this.xyPlot.getLegendWidget().getTextPaint().setTextSize(20);
        this.xyPlot.getTitleWidget().getLabelPaint().setTextSize(20);
        this.xyPlot.getTitleWidget().getLabelPaint().setColor(Color.BLACK);
        this.xyPlot.setRangeBoundaries(-0.004, 0.004, BoundaryMode.AUTO);
        this.xyPlot.setRangeStep(XYStepMode.SUBDIVIDE, 5.0);
    }

    void setXyPlotDomainIncrement(double domainIncrement) {
        this.xyPlot.setDomainStepValue(domainIncrement);
    }
}