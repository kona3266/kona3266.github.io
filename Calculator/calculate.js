window.onload = function () {
    clickFun();
};
function clickFun() {
    var equals = document.getElementById("equals");     //等于号
    var remove = document.getElementById("remove");     //删除符号
    var res =  document.getElementById("res");
    var express = document.getElementById("express");//计算表达式


    var close = document.getElementById("close"),       //关闭按钮
        max = document.getElementById("max");          //最大化按钮
    max.onmouseover = close.onmouseover = function () {
        this.innerHTML = this.dataset.ico;
    };
    max.onmouseout = close.onmouseout = function () {
        this.innerHTML = "&nbsp;";
    };

    /***********点击键盘***********/
    var keyBorders = document.querySelectorAll("#key span"),
        keyBorde = null;        //键盘
    var preKey = "",            //上一次按的键盘
        isFromHistory = false;  //是否来自历史记录
    //符号
    var symbol = {"+":"+","-":"-","×":"*","÷":"/","%":"%","=":"="};

    /***********键盘按钮***********/
    for(var j=0; j <keyBorders.length; j++){
        keyBorde = keyBorders[j];

        keyBorde.onclick = function() {
            var number = this.dataset["number"];
            clickNumber(number);
        };

    };
    function clickNumber(number) {
        var resVal = res.innerHTML;		//结果
        var exp = express.innerHTML;	//表达式
        //表达式最后一位的符号
        var expressEndSymbol = exp.substring(exp.length-1,exp.length);
        //点击的不是删除键和复位键时才能进行输入
        if(number !== "←" && number !== "C"){
            // indexOf()方法对大小写敏感，如果没有查到该字符串，则返回-1；否则，返回字符串出现的起始位置。
            var hasPoint = (resVal.indexOf('.') !== -1)?true:false; //判断result表达式里面是否含有小数点
            //如果字符串已经含有小数点，再次输入小数点时，分下面两种情况
            if(hasPoint && number === '.'){
                //上一个字符如果是符号，此时允许输入小数点，显示为0.xxx的形式
                if(symbol[preKey]){
                    res.innerHTML = "0";
                }else{
                    return false;      //上一个字符不是符号，此刻不允许输入小数点。
                }
            }
            //转换显示符号,number是从html里面得到的属性值（比如*，/），要把它转换成屏幕上的显示值（×，÷）
            //replace里面是正则表达式，/要替换的内容/g ,g代表的全局替换，就是替换所有匹配的子串，而不是第一个子串
            if(isNaN(number)){
                number = number.replace(/\*/g,"×").replace(/\//g,"÷");
            }
            //如果输入的都是数字，那么当输入达到固定长度时不能再输入了
            if(!symbol[number] && isResOverflow(resVal.length+1)){
                return false;
            }
            //点击的是符号
            //计算上一次的结果
            if(symbol[number]){
                //上一次点击的是不是符号键
                if(preKey !== "=" && symbol[preKey]){
                    express.innerHTML = exp.slice(0,-1) + number;
                }else{
                    if(exp == ""){
                        express.innerHTML = resVal + number;
                    }else{
                        express.innerHTML += resVal + number;
                    }
                    if(symbol[expressEndSymbol]){
                        exp = express.innerHTML.replace(/×/g,"*").replace(/÷/,"/");
                        res.innerHTML = eval(exp.slice(0,-1));
                    }
                }
            }else{
                //如果首位是符号，0
                if((symbol[number] || symbol[preKey] || resVal=="0") && number !== '.'){
                    res.innerHTML = "";
                }
                res.innerHTML += number;
            }
            preKey = number;

        }
    }
    /***********复位操作***********/
    var resetBtn = document.getElementById("reset");       //复位按钮
    resetBtn.onclick = function(){
        res.innerHTML = "0";
        express.innerHTML = "";
    };
    /***********减位操作***********/
    remove.onclick = function(){
        var tempRes = res.innerHTML;
        if(tempRes.length>1){
            tempRes = tempRes.slice(0,-1);
            res.innerHTML = tempRes;
        }else{
            res.innerHTML = "0";
        }
    };
    function isResOverflow(leng) {
        var calc = document.getElementById("container");
        var w = calc.style.width || getComputedStyle(calc).width || calc.currentStyle.width;
        w = parseInt(w);
        if (leng > 16) {
            return true;
        }
        return false;
    }

}