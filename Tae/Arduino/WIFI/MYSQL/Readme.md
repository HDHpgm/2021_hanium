# WiFi를 통해 MYSQL과 연동

## 설명

|                                                            |                                                        |
|------------------------------------------------------------|--------------------------------------------------------|
| MySQL_Connection                                           | MySQL에 접속할 때 사용하는 클래스                      |
| MySQL_Cursor                                               | MySQL에 접속할 인스턴스를 지정하는 클래스              |
| (MySQL_Connection).connect(서버IP, 포트, 계정명, 비밀번호) | MySQL에 접속하는 함수 (boolean)                        |
| (MySQL_Cursor)->execute(쿼리구문)                          | MySQL 구문을 실행(=전송)하는 함수 (boolean             |
| column_names                                               | MySQL 데이터의 각 열의 목록을 저장하는 자료형          |
| (MySQL_Cursor)->get_columns()                              | 해당 MySQL 인스턴스에서 각 열을 가져옴. (column_names) |
| row_values                                                 | DB의 특정 열에서 각 행에 따라서 정보를 저장하는 자료형 |
| (MySQL_Cursor)->get_next_row()                             | DB에서 행을 하나씩 가져와 저장하는 함수(row_values)    |
| (MySQL_Cursor)->close()                                    | 해당 인스턴스를 삭제하는 함수(void)      