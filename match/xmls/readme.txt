数据类型定义原则：
1、接口按照exchange的统一使用
2、DataType尽量少，能复用就复用，枚举值例外，命令规则需要使用
	**ID，就是唯一字符主键，类型为 IndexID
	**No，就是定义的数字类型，也有唯一性，类型为Number，或者LargerNumber，看取值范围
	**Name，就是名字，类型为 Name
	如果需要对类型有长度和大小的约束，可以定义特别类型，如Month、Day、Time等