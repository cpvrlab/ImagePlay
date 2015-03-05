Process: Arithmetic Operations
===================================================

\tableofcontents

\section Description
Combines two images using an arithmetic operation which is selectable
in the process properties.

Boolean operations need binary images as input

\section Inputs
- Image A: IPLData::IMAGE_COLOR
- Image B: IPLData::IMAGE_COLOR

\section Outputs
- Image: IPLData::IMAGE_COLOR

\section Algorithms
- Arithmetic_Add
- Arithmetic_Sub
- Arithmetic_Mul
- Arithmetic_Div
- Arithmetic_Max
- Arithmetic_Min
- Arithmetic_And
- Arithmetic_Or
- Arithmetic_Xor
- Arithmetic_Not
- Arithmetic_Atan2

\section Examples
<table>
 <tr>
     <th>Input A</th>
     <th>Input B</th>
     <th>Add</th>
     <th>Sub</th>
 </tr>
 <tr>
     <td><img src="../images/Lenna.png" width="256" /></td>
     <td><img src="../images/Moon.jpg" width="256" /></td>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
 </tr>
 <tr>
     <th>Mul</th>
     <th>Div</th>
     <th>Max</th>
     <th>Min</th>
 </tr>
 <tr>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
 </tr>
 <tr>
     <th>And</th>
     <th>Or</th>
     <th>Xor</th>
     <th>Not</th>
 </tr>
 <tr>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
 </tr>
 <tr>
     <th>Atan2</th>
 </tr>
 <tr>
     <td><img src="../images/IPLArithmeticOperationsAdd.png" width="256" /></td>
 </tr>
</table>
