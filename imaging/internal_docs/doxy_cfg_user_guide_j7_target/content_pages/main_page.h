/*!
    \mainpage Imaging User Guide

	This user guide has information about using Imaging on J7 EVM platforms.

	The main parts in Imaging are listed below
    <TABLE frame="box" rules="all" cellspacing="0" width="50%" border="1" cellpadding="3">
        <TR bgcolor="lightgrey">
            <TH> Imaging Module/Block </TH>
            <TH> Description </TH>
        </TR>
        <TR>
            <TD> AEWB Node </TD>
            <TD> 2A (Auto Exposure & Auto White Balance) node compliant with TIOVX framework. The node interfaces with OpenVX Graph to recieve H3A buffer and returns the output of 2A algorithms to the graph. </TD>
        </TR>
        <TR>
            <TD> AWB Algorithm </TD>
            <TD> Auto White Balance algorithm : computes color gains and estimates color temperature. Does not apply the results. The results are expected to be applied by VISS node </TD>
        </TR>
        <TR>
            <TD> AE Algorithm </TD>
            <TD> Auto Exposure algorithm : computes sensor exposure time and analog gain. Does not apply the results. The results are expected to be applied by the sensor driver </TD>
        </TR>
        <TR>
            <TD> DCC Parser </TD>
            <TD> Dynamic Camera Configuration Parser which derives tuning parameters from the tuning database provided by the application </TD>
        </TR>
        <TR>
            <TD> Sensor Driver </TD>
            <TD> Sensor driver has a common part and a sensor-specific part. Customers are not expected to modify the common part. However, the sensor specific part is extensible to add new sensors or modify the existing ones.

			IQ tuning parameters for a given sensor are also specified inside sensor driver as a set of DCC XML files. </TD>
        </TR>
    </TABLE>

    <BR> <HR>


    \par TIIMAGING References

    <TABLE frame="box" rules="all" cellspacing="0" width="50%" border="1" cellpadding="3">
        <TR bgcolor="lightgrey">
            <TH> Reference </TH>
            <TH> Description </TH>
        <TR>
            <TD> \ref BUILD_INSTRUCTIONS </TD>
            <TD> Build Instructions </TD>
        </TR>
        <TR>
            <TD> \ref group_vision_function_imaging </TD>
            <TD> Imaging API </TD>
        </TR>
    </TABLE>

	<BR> <HR>


    \par Disclaimer

    See also \ref TI_DISCLAIMER.
 */
