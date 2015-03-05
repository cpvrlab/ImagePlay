\page ConvertToGray Process: Convert To Gray

\tableofcontents

\section Description
Converts an RGB image to Grayscale.

Channel weights:
- Red: 0.2125
- Green: 0.7154
- Blue: 0.0721

\section Inputs
- Image: IPLData::IMAGE_COLOR

\section Outputs
- Image: IPLData::IMAGE_GRAYSCALE

\section Examples
<table>
 <tr>
     <th>Input</th>
     <th>Output</th>
 </tr>
 <tr>
     <td><img src="../images/Lenna.png" width="256" /></td>
     <td><img src="../images/IPLConvertToGray.png" width="256" /></td>
 </tr>
</table>