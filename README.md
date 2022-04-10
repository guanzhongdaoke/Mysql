# Mysql
#### 推荐使用mysql c++ connector
#### 简单的SQL语句可以使用Statement，复杂的拼接推荐使用Prepare Statement
# Statement
#### 最原始简单的文本SQL执行模式，需要开发者自行拼接SQL
#### 只需要new一次Statement，以后全部复用
#### 方便实现插入的BATCH: insert into values(),(),()....
#### 存在SQL注入风险,字符串一把需要屏蔽(',")
#### 二进制没有办法处理
# Prepare Statement
#### 二进制预编译SQL执行模式，无需拼接
#### 安全，无注入风险
#### 方便二进制处理
#### BATCH: 可以一次编译，多次执行相同的SQL语句，仅仅参数不同（不需要在对SQL进行语法解析了）
#### 相关SQL语句可以预先编译，后续直接复用
#### 注意预编译SQL语句上限报错，对应数据库的max_prepared_stmt_count

