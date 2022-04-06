
~~~
function test()
{
    let sheet = Application.Worksheets.Item("Sheet1")
    sheet.Rows.Item(1).Font.Bold = true
    sheet.Range("a1:d1").Value2 = ["Name", "Full Name", "Title", "Installed"]
    for(let i = 1; i <= Application.AddIns.Count; i++) {
        sheet.Cells(i + 1, 1).Value2 = Application.AddIns.Item(i).Name
        sheet.Cells(i + 1, 2).Value2 = Application.AddIns.Item(i).FullName
        sheet.Cells(i + 1, 3).Value2 = Application.AddIns.Item(i).Title
        sheet.Cells(i + 1, 4).Value2 = Application.AddIns.Item(i).Installed
    }
    sheet.Range("a1").CurrentRegion.Columns.AutoFit()
}
~~~





1.新增Sheet页，并修改名称
~~~
  Worksheets.Add(); //新开一个EXCEL
  ActiveSheet.Name = "test"; //设置Sheet页名称
~~~

2.激活Sheet页

~~~
	Worksheets.Item("Sheet页名称").Activate();

~~~


3.获取指定单元格

~~~
Cells.Item(第几行, 第几列)
~~~

4.判断此单元格是否隐藏

~~~
//false为可见，true为隐藏
Cells.Item(第几行, 第几列).EntireRow.Hidden
~~~

5.获取此单元格的值

~~~
Cells.Item(第几行, 第几列).Value2
~~~

6.获取可见行的总行数

~~~
Worksheets.Count
~~~










~~~
function helloWPS(){
	Console.log("hello wps!!");
}
 
 
function helloWorld(){
	alert("hello wps");
}
 
function helloWorld2(){
	Debug.Print("hello world2!!")
}
~~~

6.常用输出信息的方法

~~~
 
Console.log("调试信息1") //在wps编辑器中首字母需大写
Debug.Print("hello world2!!") //在wps编辑器中首字母需大写
alert("调试信息3")
Console.clear() //清空所有日志
~~~


7.1 给每个表格的第一行加上底色
~~~
function changeColorForFirstRowOfEachTable(){
	//去除所有表格第一行的阴影部分
	let count = Application.ActiveDocument.Tables.Count;
	for(let i=1;i<count;i++){
		 let table = Application.ActiveDocument.Tables.Item(i);
         // 颜色参考 https://docs.microsoft.com/zh-cn/office/vba/api/word.wdcolor
	     table.Rows.Item(1).Cells.Shading.BackgroundPatternColor = wdColorGray30
	}
}
~~~

7.2 每个表格的单元格进行处理 => 去掉首位空格 、去掉收尾回车 、居中
~~~
/*
  三重循环，有点恶心了，但是没办法
*/
function editCellsForTable(){
	//清理日志
	Console.clear()
	let count = Application.ActiveDocument.Tables.Count;
	
	for(let i=1;i<=count;i++){
		 let table = Application.ActiveDocument.Tables.Item(i);
       
	    let rowCount = table.Rows.Count;
	    for(let j=1;j<=rowCount;j++){
	    	let rowContent = table.Rows.Item(j);
	    	let cellCount = table.Rows.Item(j).Cells.Count;
	    	for (let k=1;k<=cellCount;k++){
	    		// 获取每一行的celss
	    		 let cell = table.Rows.Item(j).Cells.Item(k);
	    		  
	    		 let textVal = cell.Range.Text;
	    		 Console.log("|"+textVal+"|")
			     // 去空格  去换行符 
			     let val = textVal.replace(/[\r\n]/g,"").replace(/(^\s*)|(\s*$)/g, "");
			     cell.Range.Text=val
	    	}
	    }
	    
	        // 选中整个table  进行居中
	        Selection.SetRange(table.Range.Start,table.Range.End);
	    	Selection.ParagraphFormat.Alignment = wdAlignParagraphCenter;
	  
	}
}
~~~

7.3 格式化每一个table的表头

        思路： 选中某一行，点击格式刷，再应用到其他行
~~~
function formatTableRowExceptionFirst(){
  //选中某一行  此数据时由录制宏中生成的函数中摘取
  Selection.SetRange(4511, 4544);
  //复制选中行格式
  Selection.CopyFormat();
  //准备遍历表格
  let tabNum = 	Application.ActiveDocument.Tables.Count;
  for(let i=100;i<tabNum;i++){
    //当前表格
  	let currTab = ActiveDocument.Tables.Item(i);
    let rowNum = currTab.Rows.Count;
    for(let j=2;j<=rowNum;j++){
      let rowRange =    	currTab.Rows.Item(j).Range;
    	//选中此行，应用样式
        Selection.SetRange(rowRange.Start,rowRange.End);
    	Selection.PasteFormat()
    }
  }
}
~~~

~~~
ThisWorkbook //当前工作簿
ActiveWorkbook //活动工作簿
Workbooks('工作簿名字')
Workbooks(n) //索引号工作簿对象

常用属性：Path,Name,FullName


ActiveSheet //活动工作薄中的活动工作表
Worksheets('工作表名字') //用工作表名表示
Worksheets(n) //索引号工作表对象
Sheets('工作表名') //用工作表名表示
Sheets(n) ////索引号工作表名表示

常用属性：Name

Range("a1:c3")//单元格区域表示，能表示任何单元格
Cells(1,1) //只能表示单个单元格，无成员提示
单元格对象值属性读取写入：
只读：Value()(方法)
可读写：Value2(属性)
可读写：Formula(属性)有公式则读取公式

读取单元格的值:
alert(Range("b2").Value2())
写入单元格的值
Range("b2").Value2 = 100


~~~
JavaScript 使用关键字 var 来定义变量， 使用等号来为变量赋值：