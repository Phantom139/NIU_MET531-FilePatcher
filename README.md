#MET 531 File Patcher#
##Robert C. Fritzen##
###Current Version: Beta (b) 1.05###

##Instructions##
1. Download the .zip file.
2. Extract the folder, you can modify the .cpp file.
3. To run the program, place the .exe in the SAME FOLDER as the .csv data files from MRCC.

##Instructions For Obtaining the Data, Running Program##
1. Log in to MRCC, select the station of intrest. Select the custom time period and use 10/1/1950 to current. Select the button that says "Get Tabular Data"
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/1.jpg)

2. Once the page loads, select the button that says "CSV Version".
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/2.jpg)

3. Download the CSV file to the SAME FOLDER as the executable. Name the file accordingly (The 3 letter station code works)
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/3.jpg)

4. Run the executable. Type the name of the CSV file and confirm, then select the operation to perform on the file.
 * Note: Some options require steps from the program to be completed before running.
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/4.jpg)

##Instructions for Plotting the Data##
1. Open the .CSV file in Excel. For the temperature chart, use the _scored.csv file. Select the Date column.
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/5.jpg)

2. Scroll to the end of the data, hold the [SHIFT] key and select the last cell (It will have ####### in it)
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/6.jpg)

3. Go back to the top of the file, Hold [CTRL] and then click and drag over the columns: TMAX, TMIN, THLD9, THLD0, THLD-10
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/7.jpg)

4. Scroll to the end of the data again, hold the [SHIFT] key and the click once on the last -10 point in the THLD-10 column.
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/8.jpg)

5. Go back to the top of the file (Important, unless you want to drag the chart to the top for hours), Click the insert button, then the chart button, and select the Line option under 2D Line.
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/9.jpg)

##Instructions for Fitting the Charts and Formatting##
1. Right click on the bottom axis and select "Format Axis"
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/10.jpg)

2. Under the labels category, there is a label position selection, set this to Low
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/11.jpg)

3. For some spreadsheets, the data has a START column first (This is for the program's purpose). To disable this as a column, right click on any line and click "Select Data"
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/12.jpg)

4. In the popup box that comes up, disable the checkbox with the word START
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/13.jpg)

5. To add Axis Labels, click the button that says "Add Chart Element" and select both options under Axis Titles.

6. To move the Legend to the right, click the "Add Chart Element" button and under Legend, select Right
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/14.jpg)

7. To change the line colors, right click on a line and click the dropdown box that says "Outline"
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/15.jpg)

8. To change the titles of the Lines, right click on a line, click the "Select Data" box, select the line in the left column and click Edit, you can set the name with the top box, then click OK.
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/16.jpg)

##Other Charts##
* For the Threshold analysis, you plot the date versus the three probability columns
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/17.jpg)

##Time Series##
To plot the time series as needed by our project, you'll need to make use of the generated _monthlytres.csv file.

1. The overall steps are similar, with some minor adjustments. First, validate the data and fix any "problems" in it.
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/18.jpg)

2. Select the Year column and the Total column (ONLY DO THIS FOR 9F, DO NOT SELECT MULTIPLE DATA SERIES), Once done, select the insert button followed by the 'Recommended Charts' button. It should show you a plot with years on the bottom and values on the left, select it and click OK.
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/19.jpg)

3. To add the other series, we'll need to enter the chart's data. Right click on the chart and click the Select Data button
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/20.jpg)

4. Click the "Add" button 

5. Start by deleting both fields in the box.

6. In the lower field entry box, click the chart select button (blue with two red arrows), then scroll down to 0F and select the FIRST VALUE, then drag it over the remaining values
![](https://github.com/Phantom139/NIU_MET531-FilePatcher/blob/master/images/21.jpg)

7. Name the field and click OK.

8. Repeat steps 4 - 7 for -10F.

9. Format the chart as we did for the probability, using three shades of blue where the darker tones signify deeper thresholds.