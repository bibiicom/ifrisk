import Decimal from "decimal.js";

Decimal.set({rounding: Decimal.ROUND_DOWN});
let num1 = new Decimal("1");
let num2 = new Decimal("3");
let num3 = num1.div(num2);

console.log(num3, num3.toFixed(15));

let num11 = new Decimal("1.999999");
let num12 = new Decimal("3.666666");
console.log(num11, num11.toDecimalPlaces(3).toFixed());
console.log(num12, num12.toDecimalPlaces(10).toFixed(3));

let volume = new Decimal("10009");
let feePct = new Decimal("8");
let sub = volume.sub(volume.mul(feePct).div(100).toDecimalPlaces(5));
console.log("sub", sub.toFixed());

// 加减乘除
let num4 = num1.add(num2);
let num5 = num1.sub(num2);
let num6 = num1.mul(num2);
let num7 = num1.div(num2);
console.log("num4", num4.toFixed());
console.log("num5", num5.toFixed());
console.log("num6", num6.toFixed());
console.log("num7", num7.toFixed());

const launchpad = {
    AllVolume: new Decimal("1000"),
    MinAmountPct: new Decimal("90"),
    StockVolume: new Decimal("100"),
};

let minStockVolume = launchpad.AllVolume.mul(launchpad.MinAmountPct).div("100");
console.log("minStockVolume", minStockVolume.toFixed());
let subVolume = launchpad.AllVolume.sub(minStockVolume);
console.log("subVolume", subVolume.toFixed());
if (launchpad.StockVolume.gt(subVolume)) {
    console.log("StockVolume 小于 AllVolume - MinAmountPct，不允许申请上架");
} else {
    console.log("StockVolume 大于等于 AllVolume - MinAmountPct，允许申请上架");
}

// 整数判断
let decimal = new Decimal("3.0000000000000001");
console.log(decimal.isInt(), decimal.toFixed());



