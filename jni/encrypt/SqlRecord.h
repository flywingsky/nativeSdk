﻿//
// Copyright (C) 2010 Piotr Zagawa
//
// Released under BSD License
//

#pragma once

#include <vector>
#include "SqlCommon.h"
#include "SqlValue.h"
#include "SqlFieldSet.h"

#include "sql_macro.h"

NS_SQL_BEGIN
  
class Record {
public:
  
  // Creation and lifetime --------------------------------------------------------
  
  Record(FieldSet* fields);
  Record(Record* record);
  Record(const Record& record);
  
  // Getters/Setters --------------------------------------------------------
  
  int columnCount();
  Value* getValue(int column_index);
  Value* getValue(string fieldName);
  Value* getKeyIdValue();
  Field* fieldByName(string fieldName);
  FieldSet* fields();
  
  void setNull(int index);
  void setString(int index, string value);
  void setInteger(int index, integer value);
  void setDouble(int index, double value);
  void setBool(int index, bool value);
  void setTime(int index, time value);
  
  void setNull(string fieldName);
  void setString(string fieldName, string value);
  void setInteger(string fieldName, integer value);
  void setDouble(string fieldName, double value);
  void setBool(string fieldName, bool value);
  void setTime(string fieldName, time value);
  
  
  string toString();
  string toSql();
  bool equalsColumnValue(Record* record, string fieldName);
  bool equalsValues(Record* record);
  
public:
  string toSqlInsert(string tableName);
  string toSqlUpdate(string tableName);
  
  string toSqlInsertIfNotExists(string tableName, string key, string value);
  string toSqlUpdate(string tableName, string where_condition);
  string toSqlInsertOrReplace(string tableName);
  
private:
  friend class RecordSet;
  
  FieldSet* _fields;
  std::vector<Value> _values;
  
  void initColumnCount(int columns);
  void initColumnValue(int column_index, char* value, field_type type);
};

NS_SQL_END
