#ifndef _CDATE_H_
#define _CDATE_H_

/**CDate是表示日期的类
*  它以字符串的形式存贮日期，可以计算用于日期的计算
*/
class CDate
{
public :
	/**构造函数
	*@param pszDate 字符串表示的日期，格式为"YYYYMMDD"
	*/
	CDate(const char *pszDate);

	/**构造函数
	*@param lDate 整数表示的日期（1980-1-1至今的天数）
	*/
	CDate(int lDate);

	/**析构函数
	*/
	~CDate();

	/**取整数表示的日期
	*@return 整数表示的日期
	*/
	int ToLong();

	/**取字符串表示的日期
	*@return 字符串表示的日期
	*@remark 本方法使用了静态变量
	*/
    const char *ToString();

	/**取整数表示的星期几
	*@return 整数表示的星期几（1-7表示星期一至星期天）
	*/
	int WeekDay();

	/**取整数表示的年份
	*@return 整数表示的年份（从公元开始）
	*/
	int Year();

	/**取整数表示的月份
	*@return 整数表示的月份（1-12）
	*/
	int Month();

	/**取整数表示的月中的日期
	*@return 整数表示的月中的日期（1-31）
	*/
	int Day();

	/**CDate对象代表的日期增加一天
	*@return 增加后的对象
	*/
	CDate &operator++(int);

	/**CDate对象代表的日期减少一天
	*@return 减少后的对象
	*/
	CDate &operator--(int);
	
	/**加指定天数
	*@param i 加的天数
	*@return 加后的结果
	*/
	CDate operator+(int i);

	/**减指定天数
	*@param i 减的天数
	*@return 减后的结果
	*/
	CDate operator-(int i);
	
	/**CDate对象代表的日期增加指定天数
	*@param i 加的天数
	*@return 增加后的对象
	*/
	CDate &operator +=(int i);

	/**CDate对象代表的日期减少指定天数
	*@param i 减的天数
	*@return 减少后的对象
	*/
	CDate &operator -=(int i);
	
	/**加指定天数
	*@return 加后的结果(整数表示）
	*/
	int operator-(CDate &date);

	/**减指定天数
	*@return 减后的结果(整数表示）
	*/
	int operator-(const char* pszDate);
	
	/**判断本对象的日期与另一日期是否相等
	*@param date 要比对的另一日期
	*@return true 相等，false不相等
	*/
	bool operator==(CDate &date);

	/**判断本对象的日期与另一字符串表示的日期是否相等
	*@param pszData 要比对的字符串表示的另一日期
	*@return true 相等，false不相等
	*/
	bool operator==(const char *pszDate);
	
	/**判断一个字符串是否合法地表示日期("YYYYMMDD")
	*@param pszData
	*@return true 合法的日期表示，false 非法的日期表示
	*/
	static bool IsValid(const char *pszDate);

	/**将一个字符串转换为整数表示的日期
	*@param pszDate 要转换的字符串
	*@return 转换出的整数表示的日期
	*/
	static int DateToLong(const char *pszDate);

	//以2018年开始的转换
	static int DateFrom2020ToLong(const char *pszDate);

	/**将整数表示的日期转换为一个字符串表示的日期
	*@param lDate 要转换的整数表示的日期
	*@return 转换出的字符串表示的日期
	*@remark 本方法使用了静态变量
	*/
	static char *LongToDate(int lDate);
	
	/**判断某一年是否为闰年
	*@param nYear 年份
	*@return 1 该年份为闰年，0 该年份不是闰年
	*/
	static int IsLeapYear(int nYear);

	/**取指定日期是星期几
	*@param pszDate 指定的日期
	*@return 整数表示的星期几（1-7表示星期一至星期天）
	*/
	static int GetWeekDay(const char *pszDate);

	/**取指定月份的天数
	*@param nYear 年份
	*@param nMonth 月份
	*@return 该月份的天数
	*/
	static int GetDays(int nYear,int nMonth);
private:
	string m_strDate;		/**<用字符串表示的日期 */
};

/**CTime是表示绝对时间（时刻）的类，精确到秒
*  它以整数的形式存贮当天的秒数，可以计算用于时间的计算
*/
class CTime
{
public :
	/**构造函数
	*@param pszTime 用字符串表示的时刻，格式为HH24:MM:SS
	*/
	CTime(const char *pszTime);

	/**构造函数
	*@param lTime 用整数表示的时刻，即从零点起的秒数
	*/
	CTime(int lTime);

	/**析构函数
	*
	*/
	~CTime();

	/**判断本对象是否表示了一个合法的时刻
	*@return true 合法的时刻，false 非法的时刻
	*/
	bool IsValid();

	/**取整数表示的时刻
	*@return 整数表示的时刻
	*/
	int ToLong();
	
	/**取用字符串表示的时刻
	*@return 字符串表示的时刻
	*@remark 本方法使用了静态变量
	*/
    const char *ToString();

	/**取本时刻24小时制的小时
	*@return 本时刻24小时制的小时
	*/
	int Hour();

	/**取本时刻的分钟
	*@return 本时刻的分钟
	*/
	int Minute();

	/**取本时刻的秒钟
	*@return 本时刻的秒钟
	*/
	int Second();

	/**本时刻加上一定的时间
	*@param lTimeSpan 要加上的时间
	*@return  本时刻加上一定的时间的结果
	*@remark 超过24点会重新从0点开始 
	*/
	CTime operator+(const int lTimeSpan);
	
	/**本时刻减去一定的时间
	*@param lTimeSpan 要减去的时间
	*@return 本时刻减去一定的时间的结果
	*@remark 小于0点会重新开始 
	*/
	CTime operator-(const int lTimeSpan);

	/**计算两个时刻所差的秒数
	*@param t 被减的时刻
	*@return 两个时刻所差的秒数
	*/
	int operator-(const CTime &t);

	/**判断两个时刻是否相等
	*@param t 要比较的时刻
	*@return true 两个时刻相等，false 两个时刻不相等
	*/
	bool operator==(const CTime &t);

	/**判断两个时刻是否相等
	*@param t 要比较的用字符串表示的时刻
	*@return true 两个时刻相等，false 两个时刻不相等
	*/
	bool operator==(const char *pszTime);
	
	/**检查一个字符串是否表示了合法的时刻，即符合"HH24:MM:SS"格式
	*@param pszTime 要检查的字符串
	*@return true 合法的时刻表示，false 非法的时刻表示
	*/
	static bool IsValid(const char *pszTime);
	
	/**将字符串表示的时刻转换为整数表示的时刻
	*@param pszTime 用字符串表示的时刻，格式为HH24:MM:SS
	*@return 用整数表示的时刻，INVALID_TIME为非法的时间
	*/
	static int TimeToLong(const char *pszTime);

	//Time转化为多少分钟
	static int MinToLong(const char *pszTime);

	/**将整数表示的时刻转换为字符串表示的时刻
	*@param lTime 用整数表示的时刻，即从零点起的秒数
	*@return 用字符串表示的时刻，NULL为非法的时间
	*/
	static const char *LongToTime(int lTime);

	enum
	{
		INVALID_TIME = 0xffffffff,				//非法的时间
		MAX_TIME_VALUE = 24*3600		//一天内最大的秒数
	};
private:
	int  m_lTimeValue;			/**<用整数表示的时刻 */
};

#endif
