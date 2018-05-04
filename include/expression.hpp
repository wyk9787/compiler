#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include "global.hpp"
#include "token.hpp"
#include "type.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace fexp {

/******************************************************************************
                               Helper
*******************************************************************************/
Shared_Exp evaluate(Shared_Exp exp, bool print_step);

/******************************************************************************
                               Exp Header
*******************************************************************************/

class Exp {
public:
  virtual Shared_Exp step() = 0;
  virtual Shared_Exp substitute(std::string var, Shared_Exp t) = 0;
  virtual std::string string_of_exp() = 0;
  virtual Shared_Typ typecheck(context_t context) = 0;
  virtual cexp::Shared_Exp convert() = 0;

  virtual bool is_value() { return false; }
  virtual bool is_bool() { return false; }
  virtual bool is_int() { return false; }
  virtual bool is_float() { return false; }
  virtual bool is_unit() { return false; }
  virtual bool is_NaN() { return false; }
  virtual bool is_var() { return false; }
  virtual bool is_pair() { return false; }
  virtual bool is_list() { return false; }
  virtual bool is_struct() { return false; }

  virtual bool get_bool() {
    std::cerr << "Debug: Expecting a boolean!\n";
    exit(1);
  }
  virtual int get_int() {
    std::cerr << "Debug: Expecting an integer!\n";
    exit(1);
  }
  virtual std::string get_NaN() {
    std::cerr << "Debug: Expecting an integer!\n";
    exit(1);
  }
  virtual double get_float() {
    std::cerr << "Debug: Expecting a float!\n";
    exit(1);
  }
  virtual std::string get_var() {
    std::cerr << "Debug: Expecting a variable!\n";
    exit(1);
  }
  virtual Shared_EPair get_pair() {
    std::cerr << "Debug: Expecting a pair!\n";
    exit(1);
  }
  virtual Shared_EList get_list() {
    std::cerr << "Debug: Expecting a list!\n";
    exit(1);
  }

  virtual Shared_EPtr get_ptr() {
    std::cerr << "Debug: Expecting a pointer!\n";
    exit(1);
  }

  virtual ~Exp(){};
};

/******************************************************************************
                               EOperator Header
*******************************************************************************/

class EOperator : public Exp {
private:
  TokenKind id;
  Shared_Exp e1, e2;
  Shared_Exp evaluate_num(Shared_Exp e1, Shared_Exp e2, bool is_int);

public:
  EOperator(TokenKind _id, Shared_Exp _e1, Shared_Exp _e2);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp t);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EComp Header
*******************************************************************************/

class EComp : public Exp {
private:
  TokenKind id;
  Shared_Exp e1, e2;
  Shared_Exp evaluate_bool(Shared_Exp e1, Shared_Exp e2);

public:
  EComp(TokenKind _id, Shared_Exp _e1, Shared_Exp _e2);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp t);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               ELit Header
*******************************************************************************/

class ELit : public Exp {
private:
  bool _is_int;
  bool _is_NaN;
  int int_data;
  double float_data;

public:
  ELit(bool __is_int, int _int_data, double _float_data, bool __is_NaN);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();

  bool is_value();
  bool is_int();
  int get_int();
  bool is_float();
  double get_float();
  bool is_NaN();
  std::string get_NaN();
};

/******************************************************************************
                               EBool Header
*******************************************************************************/

class EBool : public Exp {
private:
  bool data;

public:
  EBool(bool _data);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();

  bool is_value();
  bool is_bool();
  bool get_bool();
};

/******************************************************************************
                               EVar Header
*******************************************************************************/

class EVar : public Exp {
private:
  std::string data;

public:
  EVar(std::string _data);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();

  bool is_value();
  bool is_var();
  std::string get_var();
};

/******************************************************************************
                               EUnit Header
*******************************************************************************/

class EUnit : public Exp {
private:
public:
  EUnit();
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();

  bool is_value();
  bool is_unit();
};

/******************************************************************************
                               EIf Header
*******************************************************************************/

class EIf : public Exp {
private:
  Shared_Exp e1, e2, e3;

public:
  EIf(Shared_Exp _e1, Shared_Exp _e2, Shared_Exp _e3);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               ELet Header
*******************************************************************************/

class ELet : public Exp {
private:
  std::string var;
  Shared_Typ t;
  Shared_Exp e1, e2;

public:
  ELet(std::string _var, Shared_Typ _t, Shared_Exp _e1, Shared_Exp _e2);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EApp Header
*******************************************************************************/

class EApp : public Exp {
private:
  std::string id;
  std::vector<Shared_Exp> v;

public:
  EApp(std::string _id, std::vector<Shared_Exp> _v);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EPair Header
*******************************************************************************/

class EPair : public Exp {
private:
  Shared_Exp e1;
  Shared_Exp e2;

public:
  EPair(Shared_Exp _e1, Shared_Exp _e2);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();

  bool is_value();
  bool is_pair();

  Shared_EPair get_pair();
  Shared_Exp get_first();
  Shared_Exp get_second();
};

/******************************************************************************
                               EFst Header
*******************************************************************************/

class EFst : public Exp {
private:
  Shared_Exp e;

public:
  EFst(Shared_Exp _e);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               ESnd Header
*******************************************************************************/

class ESnd : public Exp {
private:
  Shared_Exp e;

public:
  ESnd(Shared_Exp _e);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EList Header
*******************************************************************************/

class EList : public Exp {
private:
  std::vector<Shared_Exp> e_list;
  Shared_Typ t;

public:
  EList(std::vector<Shared_Exp> _e_list, Shared_Typ e);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp t);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();

  bool is_value();
  bool is_list();

  Shared_EList get_list();
  Shared_Typ get_t();
  std::vector<Shared_Exp> get_e_list();
};

/******************************************************************************
                               ECons Header
*******************************************************************************/

class ECons : public Exp {
private:
  Shared_Exp e1;
  Shared_Exp e2;

public:
  ECons(Shared_Exp _e1, Shared_Exp _e2);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               ECar Header
*******************************************************************************/

class ECar : public Exp {
private:
  Shared_Exp e;

public:
  ECar(Shared_Exp _e);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               ECdr Header
*******************************************************************************/

class ECdr : public Exp {
private:
  Shared_Exp e;

public:
  ECdr(Shared_Exp _e);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EEmpty Header
*******************************************************************************/

class EEmpty : public Exp {
private:
  Shared_Exp e;

public:
  EEmpty(Shared_Exp _e);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               ERef Header
*******************************************************************************/

class ERef : public Exp {
private:
  Shared_Exp e;

public:
  ERef(Shared_Exp _e);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EPtr Header
*******************************************************************************/

class EPtr : public Exp {
private:
  size_t n;

public:
  EPtr(size_t _n);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();

  bool is_value();
  Shared_EPtr get_ptr();
  size_t get_addr();
};

/******************************************************************************
                               EDeref Header
*******************************************************************************/

class EDeref : public Exp {
private:
  Shared_Exp e;

public:
  EDeref(Shared_Exp _e);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EAssign Header
*******************************************************************************/

class EAssign : public Exp {
private:
  Shared_Exp e1;
  Shared_Exp e2;

public:
  EAssign(Shared_Exp _e1, Shared_Exp _e2);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               ESeq Header
*******************************************************************************/

class ESeq : public Exp {
private:
  Shared_Exp e1;
  Shared_Exp e2;

public:
  ESeq(Shared_Exp _e1, Shared_Exp _e2);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EWhile Header
*******************************************************************************/

class EWhile : public Exp {
private:
  Shared_Exp e1;
  Shared_Exp e2;
  Shared_Exp e_temp;

public:
  EWhile(Shared_Exp _e1, Shared_Exp _e2, Shared_Exp _e_temp);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EDef Header
*******************************************************************************/

class EDef : public Exp {
private:
  std::string id;
  Shared_Exp e;
  Shared_Typ t;

public:
  EDef(std::string _id, Shared_Exp _e, Shared_Typ _t);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};

/******************************************************************************
                               EStruct Header
*******************************************************************************/

class EStruct : public Exp {
private:
  struct_data_t e_map;
  struct_type_t t_map;

public:
  EStruct(struct_data_t _e_map, struct_type_t _t_map);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();

  bool is_value();
  bool is_struct();
  struct_data_t get_data();
};

/******************************************************************************
                               EDot Header
*******************************************************************************/

class EDot : public Exp {
private:
  Shared_Exp e;
  std::string id;

public:
  EDot(Shared_Exp _e, std::string _id);
  Shared_Exp step();
  Shared_Exp substitute(std::string var, Shared_Exp e);
  std::string string_of_exp();
  Shared_Typ typecheck(context_t context);
  cexp::Shared_Exp convert();
};
}
#endif
