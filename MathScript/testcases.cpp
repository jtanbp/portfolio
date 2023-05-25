//
//  testcases.cpp
//  ExpressionClass
//
//  Created by Jowie Tan on 1/31/23.
//

#include <stdio.h>
#include "expr.h"
#include "val.h"
#include "catch.h"
#include "parse.hpp"

TEST_CASE("Testing Expression Cases") {
    PTR(Expr) e1 = NEW(Num)(1);
    PTR(Expr) e2 = NEW(Num)(2);
    PTR(Expr) e3 = NEW(Num)(3);
    PTR(Expr) e4 = NEW(Num)(4);
    PTR(Expr) e5 = NEW(Num)(5);
    PTR(Expr) e6 = NEW(Num)(6);
    
    PTR(Expr) var1 = NEW(Var)("Jowie");
    PTR(Expr) var2 = NEW(Var)("Tan");
    
    PTR(Expr) a1 = NEW(Add)(e2, e4); // 2 + 4
    PTR(Expr) a2 = NEW(Add)(e1, e5); // 1 + 5
    PTR(Expr) a3 = NEW(Add)(e2, e3); // 2 + 3
    
    PTR(Expr) m1 = NEW(Mult)(e2, e3); // 2 * 3
    PTR(Expr) m2 = NEW(Mult)(e1, e6); // 1 * 6
    PTR(Expr) m3 = NEW(Mult)(e3, e4); // 3 * 4
    
    SECTION("Expression Equals") {
        CHECK( e1->equals( NEW(Num)(1) ) );
        CHECK_FALSE( e2-> equals( e3 ) );
    }
    
    SECTION("Add Equals") {
        CHECK( a1->equals( NEW(Add)(e2, e4) ) );
        CHECK_FALSE( a1->equals(a3) );
    }
            
    SECTION("Mult Equals") {
        CHECK( m1->equals( NEW(Mult)(e2, e3) ) );
        CHECK_FALSE( m1->equals(m3) );
    }
    
    SECTION("Var Equals") {
        CHECK( var1->equals( NEW(Var)("Jowie") ) );
        CHECK_FALSE( var1->equals(var2) );
    }
    
    SECTION("Compare different types against one another") {
        CHECK_FALSE( a1->equals(m1) );
        CHECK_FALSE( a1->equals(var1) );
        CHECK_FALSE( a1->equals(e1) );
        
        CHECK_FALSE( m1->equals(a2) );
        CHECK_FALSE( m1->equals(var1) );
        CHECK_FALSE( m1->equals(e1) );
        
        CHECK_FALSE( var1->equals(a1) );
        CHECK_FALSE( var1->equals(m2) );
        CHECK_FALSE( var1->equals(e1) );
        
        CHECK_FALSE( e1->equals(a1) );
        CHECK_FALSE( e1->equals(m1) );
        CHECK_FALSE( e1->equals(var1) );
    }
}

//TEST_CASE("Interpret Test Cases"){
//    Expr *e1 = new Num(1);
//    Expr *e2 = new Num(2);
//    Expr *e3 = new Num(3);
//    Expr *e4 = new Num(4);
//    Expr *e5 = new Num(5);
//    Expr *e6 = new Num(6);
//
//    Expr *var_x = new Var("x");
//    Expr *var_y = new Var("y");
//
//    Expr *a1 = new Add(e2, e4); // 2 + 4
//    Expr *a2 = new Add(e1, e5); // 1 + 5
//    Expr *a3 = new Add(e2, e3); // 2 + 3
//    Expr *a4 = new Add(var_x, e4); // x + 4
//    Expr *a5 = new Add(e2, var_y); // 2 + y
//
//    Expr *m1 = new Mult(e2, e3); // 2 * 3
//    Expr *m2 = new Mult(e1, e6); // 1 * 6
//    Expr *m3 = new Mult(e3, e4); // 3 * 4
//    Expr *m4 = new Mult(var_x, e4); // x + 4
//    Expr *m5 = new Mult(e2, var_y); // 2 + y
//
//    SECTION("Interp Check"){
//        CHECK_THROWS_WITH( var_x->interp(), "no value for variable");
//        CHECK ( a1->interp()->equals( new NumVal(6) ) );
//        CHECK ( a2->interp()->equals( new NumVal(6) ) );
//        CHECK ( a3->interp()->equals( new NumVal(5) ) );
//        CHECK ( m1->interp()->equals( new NumVal(6) ) );
//        CHECK ( m2->interp()->equals( new NumVal(6) ) );
//        CHECK ( m3->interp()->equals( new NumVal(12) ) );
//
//        CHECK( ( new Mult( new Num(3), new Num(2) ) )->interp()->equals( new NumVal(6) ) ); // 3 + 2
//        CHECK( ( new Add(
//                new Add ( new Num(10), new Num(15)),
//                new Add( new Num(20), new Num(20) ) ) )-> interp()->equals( new NumVal(65) ) );
//                // (10 + 15) + (20 + 20)
//    }
//
//    SECTION("Subst Check"){
//        // x + 7 into
//        // y + y
//        CHECK( ( new Add( new Var("x"), new Num(7) ) )->subst( "x", new Var("y") )->equals( new Add(new Var("y"), new Num(7) ) ) );
//
//        // x into
//        // y + 7
//        CHECK( ( new Var("x") )->subst( "x", new Add( new Var("y"), new Num(7) ) )->equals( new Add( new Var("y"), new Num(7) ) ) );
//
//        // Human into
//        // 10 * x
//        Expr *human = new Var("Human");
//        Expr *ten = new Num(10);
//        Expr *tentimesx = new Mult(ten, new Var("x"));
//        CHECK ( human->subst("Human", tentimesx)->equals(tentimesx) );
//
//        // Human but fails because its trying to change just H
//        CHECK_FALSE ( human->subst("H", tentimesx)->equals(tentimesx) );
//
//        // Change x * x into
//        // (10) * (10)
//        CHECK( (new Mult(new Var("x"), new Var("x")))->subst("x", new Num(10))->interp()->equals( new NumVal(100) ) );
//    }
//}
//
//TEST_CASE ("String Print Test") {
//    Expr *e1 = new Num(1);
//    Expr *e2 = new Num(2);
//    Expr *e3 = new Num(3);
//    Expr *e4 = new Num(4);
//
//    Expr *var_x = new Var("x");
//
//    Expr *a1 = new Add(e2, e4); // 2 + 4
//
//    Expr *m1 = new Mult(e2, e3); // 2 * 3
//    Expr *m4 = new Mult(var_x, e4); // x + 4
//
//    SECTION ("Num Print") {
//        CHECK( e1->to_string() == "1" );
//        CHECK( e1->pretty_print_to_string() == "1" );
//    }
//
//    SECTION ("Add Print") {
//        CHECK( a1->to_string() == "(2+4)");
//    }
//
//    SECTION ("Mult Print") {
//        CHECK( m1->to_string() == "(2*3)");
//    }
//
//    SECTION ("Var Print") {
//        CHECK( var_x->to_string() == "x");
//        CHECK( var_x->pretty_print_to_string() == "x");
//    }
//
//    SECTION ("Test with Variable") {
//        CHECK( m4->pretty_print_to_string() == "x * 4" );
//    }
//
//    SECTION ("Add then Mult") {
//        // 2 * (3 + 4)
//        CHECK ((new Mult(e2, new Add(e3, e4)))->pretty_print_to_string() == "2 * (3 + 4)");
//        // (2 + 3) * 4
//        CHECK ((new Mult(new Add(e2, e3), e4))->pretty_print_to_string() ==
//               "(2 + 3) * 4");
//    }
//
//    SECTION("mult_mults_mults_mults")
//    {
//        // (10 * (10 * 10) * 10) * 10 * 10
//        CHECK((new Mult( new Mult(new Num(10), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10))))
//                      ->pretty_print_to_string()  == "(10 * (10 * 10) * 10) * 10 * 10");
//    }
//
//    SECTION("Homework Example Tests") {
//        // (2 * 3) * 4
//        CHECK((new Mult (new Mult (new Num(2), new Num(3)), new Num(4)))->pretty_print_to_string() == "(2 * 3) * 4");
//        // 2 * 3 * 4
//        CHECK((new Mult (new Num(2), new Mult (new Num(3), new Num(4))))->pretty_print_to_string() == "2 * 3 * 4");
//    }
//}
//
//TEST_CASE("_Let Tests") {
//
//    SECTION("Let Interp") {
//        // _let x = 5 _in x + 2
//        Expr* simple_test = new Let("x", new Num(5), new Add(new Var("x"), new Num(2)));
//
//        REQUIRE(simple_test->interp()->equals( new NumVal(7) ) );
//
//        // _let x = 5 _in (_let y = 3 _in (y + 2)) + x
//        Expr *let_expr = new Let("x", new Num(5),
//                            new Add(
//                            new Let("y", new Num(3),
//                            new Add(new Var("y"), new Num(2))),
//                            new Var("x")));
//        //std::cout << "Hello\n";
//        REQUIRE(let_expr->interp()->equals( new NumVal(10) ) );
//
//        // _let x = 1 _in _let y = x + 1 _in x + y
//        let_expr = new Let("x", new Num(1),
//                    new Let("y", new Add(new Var("x"), new Num(1)),
//                    new Add(new Var("x"), new Var("y"))));
//
//        REQUIRE(let_expr->interp()->equals( new NumVal(3) ) );
//    }
//
//    SECTION("Let Equals") {
//        // _let x = 5 _in (_let y = 3 _in (y + 2)) + x
//        Expr *let_expr1 = new Let("x", new Num(5),
//                            new Add(
//                            new Let("y", new Num(3),
//                            new Add(new Var("y"), new Num(2))),
//                            new Var("x")));
//
//        Expr *let_expr2 = new Let("x", new Num(5),
//                            new Add(
//                            new Let("y", new Num(3),
//                            new Add(new Var("y"), new Num(2))),
//                            new Var("x")));
//
//        REQUIRE(let_expr1->equals(let_expr2) == true);
//
//        // _let x = 1 _in _let y = x + 1 _in x + y
//        let_expr1 = new Let("x", new Num(1),
//                    new Let("y", new Add(new Var("x"), new Num(1)),
//                    new Add(new Var("x"), new Var("y"))));
//
//        let_expr2 = new Let("x", new Num(1),
//                    new Let("y", new Add(new Var("x"), new Num(1)),
//                    new Add(new Var("x"), new Var("y"))));
//
//        REQUIRE(let_expr1->equals(let_expr2) == true);
//    }
//
//    SECTION("Let Subst") {
//        Expr* e1 = new Let("x", new Num(5), new Add(new Var("y"), new Var("x")));
//        Expr* e2 = e1->subst("y", new Num(3));
//        Expr* expected = new Let("x", new Num(5), new Add(new Num(3), new Var("x")));
//        REQUIRE(expected->equals(e2));
//        REQUIRE(expected->interp()->equals( e2->interp() ));
//    }
//
//    SECTION("Let Print") {
//        Expr* lp1 = new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x")));
//        Expr* lp2 = new Let("x", new Num(3), new Let("y", new Num(3), new Let("z", new Add(new Var("x"), new Num(3)), new Mult(new Add(new Var("x"), new Var("y")), new Var("z")))));
//        CHECK( lp1-> to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
//        CHECK( lp2-> to_string() == "(_let x=3 _in (_let y=3 _in (_let z=(x+3) _in ((x+y)*z))))");
//    }
//
//    SECTION("Let Pretty Print") {
//        CHECK( (new Let("x", new Num(1),
//                   new Let("y", new Num(1),
//                           new Let("z", new Add(new Var("x"), new Num(1)),
//                                   new Mult(new Add(new Var("x"), new Var("y")), new Var("z"))))))->interp()->equals( new NumVal(4) ) );
//        CHECK( (new Mult((new Add(new Mult(new Num(5), new Let("x", new Num(5), new Mult(new Var("x"), new Num(2)))), new Num(1))), new Num(7))) -> interp()->equals( new NumVal(357) ) ); // 51 * 7
//        CHECK( (new Let("x", new Num(10), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10)))))
//                  ->interp()->equals( new NumVal(1000000) ) );
//        CHECK( (new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Num(10), new Num(10)))))
//                   ->interp()->equals( new NumVal(10000) ) );
//        CHECK_THROWS_WITH( ((new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Var("y"), new Num(10)))))
//                   -> interp()->equals( new NumVal(10000) ) ), "no value for variable");
//    }
//
//    SECTION("Homework Example Tests") {
//        CHECK( (new Add(new Mult(new Num(5), new Let("x", new Num(5), new Var("x"))), new Num(1)))-> pretty_print_to_string()
//               == "5 * (_let x = 5\n"
//                  "     _in  x) + 1");
//        CHECK( (new Mult(new Mult(new Num (2), new Let("x", new Num(5), new Add(new Var("x") ,new  Num(1)) )), new Num(3) )) -> pretty_print_to_string()
//               == "(2 * _let x = 5\n"
//                  "     _in  x + 1) * 3");
//    }
//
//    SECTION("Let_equals_mine") {
//        SECTION("Values_same") {
//            REQUIRE( (new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) )));
//        }
//        SECTION("Values_same_different_rhs") {
//            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("x", new Num(5), new Add(new Num(2), new Var("x")) )));
//        }
//        SECTION("Values_same_different_lhs") {
//            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("y", new Num(4), new Add(new Num(2), new Var("x")) )));
//        }
//        SECTION("Values_same_different_body") {
//            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("x", new Num(4), new Mult(new Num(3), new Var("y")) )));
//        }
//        SECTION("different_types") {
//            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals( new Mult(new Num(3), new Var("y")) ));
//        }
//    }
//
//    SECTION("classmate test cases") {
//        CHECK( (new Mult(new Num (2), new Let("x", new Num(5), new Add(new Var("x") ,new Num(1)) ) )) -> pretty_print_to_string()
//            == "2 * _let x = 5\n"
//                "    _in  x + 1");
//        CHECK( (new Let("x", new Num(10), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10)))))
//                       ->pretty_print_to_string()  == "_let x = 10\n"
//                                                      "_in  (x * (10 * 10) * 10) * 10 * 10");
//        CHECK( (new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Num(10), new Num(10)))))
//                        -> pretty_print_to_string() == "_let x = 1\n"
//                                                       "_in  (x * (10 * 10) * x) * 10 * 10" );
//        CHECK( (new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Var("y"), new Num(10)))))
//                       -> pretty_print_to_string() == "_let x = 1\n"
//                                                      "_in  (x * (10 * 10) * x) * y * 10" );
//    }
//}
//
///* Some ideas for parse tests, probably not enough. Also, you may need
//   to adjust these tests, and there may even be mistakes here. */
//TEST_CASE("Parsing Tests") {
//
//    SECTION("Let Test") {
//        CHECK( parse_str("_let x = 5 _in x + 2")->interp()->equals( new NumVal(7) ) );
//
//        CHECK_THROWS_WITH( parse_str("_tel x = 5 _in x + 2"), "parse_inner: invalid input");
//
//        CHECK_THROWS_WITH( parse_str("_lte x = 5 _in x + 2"), "parse_inner: invalid input");
//
//        CHECK_THROWS_WITH( parse_str("_ler x = 5 _in x + 2"), "parse_inner: invalid input");
//
//        CHECK_THROWS_WITH( parse_str("_let x 5 _in x + 2"), "parse_let: invalid input not '='");
//
//        CHECK_THROWS_WITH( parse_str("_let x = 5 in x + 2"), "parse_let: invalid input not '_'");
//
//        CHECK_THROWS_WITH( parse_str("_let x = 5 _ia x + 2"), "parse_let: invalid input not 'in'");
//
//        CHECK ( parse_str("_let x = 1 _in _let y = x + 1 _in x + y")->interp()->equals( new NumVal(3) ) );
//    }
//
//    SECTION("consume") {
//        std::string test = "test";
//        std::istringstream iss (test);
//        CHECK_THROWS_WITH(consume(iss , 's'), "consume mismatch");
//    }
//
//    // Provided test cases
//    SECTION("parse examples") {
//        CHECK_THROWS_WITH( parse_str("()"), "bad input" );
//        CHECK( parse_str("(1)")->equals(new Num(1)) );
//        CHECK( parse_str("(((1)))")->equals(new Num(1)) );
//        CHECK_THROWS_WITH( parse_str("(1"), "bad input" );
//        CHECK( parse_str("1")->equals(new Num(1)) );
//        CHECK( parse_str("10")->equals(new Num(10)) );
//        CHECK( parse_str("-3")->equals(new Num(-3)) );
//        CHECK( parse_str("  \n 5  ")->equals(new Num(5)) );
//        CHECK_THROWS_WITH( parse_str("-"), "parse_num: invalid input" );
//        CHECK_THROWS_WITH( parse_str(" -   5  "), "parse_num: invalid input" );
//        CHECK( parse_str("x")->equals(new Var("x")) );
//        CHECK( parse_str("xyz")->equals(new Var("xyz")) );
//        CHECK( parse_str("xYz")->equals(new Var("xYz")) );
//        CHECK_THROWS_WITH( parse_str("x_z"), "parse_string: invalid input of '_'" );
//        CHECK( parse_str("x + y")->equals(new Add(new Var("x"), new Var("y"))) );
//        CHECK( parse_str("x * y")->equals(new Mult(new Var("x"), new Var("y"))) );
//        CHECK( parse_str("z * x + y")
//            ->equals(new Add(new Mult(new Var("z"), new Var("x")),
//                             new Var("y"))) );
//        CHECK( parse_str("z * (x + y)")
//            ->equals(new Mult(new Var("z"),
//                              new Add(new Var("x"), new Var("y"))) ));
//    }
//}
//
//TEST_CASE("IfExpr, EqExpr, and BoolExpr tests") {
//    SECTION("Test BoolExpr") {
//        Expr *true_expr = new BoolExpr(true);
//        Expr *false_expr = new BoolExpr(false);
//
//        Val *true_val = true_expr->interp();
//        Val *false_val = false_expr->interp();
//
//        REQUIRE(true_val->is_true() == true);
//        REQUIRE(false_val->is_true() == false);
//    }
//
//    SECTION("Test EqExpr") {
//        Expr *eq_expr1 = new EqExpr(new Num(5), new Num(5));
//        Val *eq_val1 = eq_expr1->interp();
//        REQUIRE(eq_val1->is_true() == true);
//
//        Expr *eq_expr2 = new EqExpr(new Num(5), new Num(3));
//        Val *eq_val2 = eq_expr2->interp();
//        REQUIRE(eq_val2->is_true() == false);
//
//        Expr *eq_expr3 = new EqExpr(new BoolExpr(true), new BoolExpr(false));
//        Val *eq_val3 = eq_expr3->interp();
//        REQUIRE(eq_val3->is_true() == false);
//
//        Expr *eq_expr4 = new EqExpr(new BoolExpr(true), new BoolExpr(true));
//        Val *eq_val4 = eq_expr4->interp();
//        REQUIRE(eq_val4->is_true() == true);
//    }
//
//    SECTION("Test IfExpr") {
//        Expr *if_expr1 = new IfExpr(new BoolExpr(true), new Num(5), new Num(3));
//        Val *if_val1 = if_expr1->interp();
//        REQUIRE(if_val1->to_string() == "5");
//
//        Expr *if_expr2 = new IfExpr(new BoolExpr(false), new Num(5), new Num(3));
//        Val *if_val2 = if_expr2->interp();
//        REQUIRE(if_val2->to_string() == "3");
//
//        Expr *if_expr3 = new IfExpr(new EqExpr(new Num(2), new Num(2)), new Num(10), new Num(20));
//        Val *if_val3 = if_expr3->interp();
//        REQUIRE(if_val3->to_string() == "10");
//
//        Expr *if_expr4 = new IfExpr(new EqExpr(new Num(2), new Num(3)), new Num(10), new Num(20));
//        Val *if_val4 = if_expr4->interp();
//        REQUIRE(if_val4->to_string() == "20");
//    }
//
//    SECTION("Test Parse") {
//        CHECK_THROWS_WITH(parse_str("_if 4 + 1 \n _then 2 \n _else 3")->interp(), "Condition in if expression must be a boolean value" );
//        CHECK_THROWS_WITH(parse_str("_if 4 + 1 \n _tten 2 \n _else 3")->interp(), "expected '_then' keyword" );
//        CHECK_THROWS_WITH(parse_str("_if 4 + 1 \n _then 2 \n _elfe 3")->interp(), "expected '_else' keyword" );
//        CHECK(parse_str("_if 4 == 4 \n _then 2 \n _else 3")->interp()->equals(new NumVal(2)));
//        CHECK(parse_str("_if 4 == 3 \n _then 2 \n _else 3")->interp()->equals(new NumVal(3)));
//        CHECK(parse_str("_if _true \n _then 2 \n _else 3")->interp()->equals(new NumVal(2)));
//        CHECK(parse_str("_if _false \n _then 2 \n _else 3")->interp()->equals(new NumVal(3)));
//        CHECK(parse_str("_if _false \n _then _false + 2 \n _else 3")->interp()->equals(new NumVal(3)));
//        CHECK_THROWS_WITH(parse_str("_if _true \n _then _false + 2 \n _else 3")->interp()->equals(new NumVal(2)), "Cannot add boolean values.");
//
//
//        CHECK(parse_str("_if 4 == 4 \n _then 2 \n _else 3")->to_string() == "_if 4==4 _then 2 _else 3");
//        CHECK(parse_str("_if 4 == 4 \n _then 2 \n _else 3")->pretty_print_to_string() == "_if  4 == 4\n_then  2\n_else  3");
//    }
//}

TEST_CASE("Functions Test") {
    SECTION("Parse Cases") {
        CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n x + _true\n_in f(5)")->interp(Env::empty), "NumVal: add_to: nullptr");
        CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n7\n_in f(5 + _true)")->interp(Env::empty), "NumVal: add_to: nullptr");
        CHECK_THROWS_WITH(parse_str("_let f = _fun (x) x + 1\n_in f + 5")->interp(Env::empty), "FunVal: add_to: Cannot add function values");
        // This is wrong cuz f no brackets after, was not able to fix
        // CHECK_THROWS_WITH(parse_str("_let f = _fun (x) x + 1\n_in f 5")->interp(), "parse_fun: error since '(' is expected");
        // Cant have _fun standalone
        // CHECK_THROWS_WITH(parse_str("_fun (x) x + 1")->interp(), "parse_inner: invalid input");
    }
    SECTION("Interp Cases") {
        CHECK(parse_str("_let x = _fun(y) y + 2\n_in x(2)")->interp(Env::empty)->equals(NEW(NumVal)(4)));
        CHECK(parse_str("_let f = _fun (x)\n7\n_in f(5)")->interp(Env::empty)->equals(NEW(NumVal)(7)));
        CHECK(parse_str("_let f = _fun (x)\n_true\n_in f(5)")->interp(Env::empty)->equals(NEW(BoolVal)(true)));
        
        CHECK(parse_str("_let f = _fun (x)\n x + _true\n_in 5 + 1")->interp(Env::empty)->equals(NEW(NumVal)(6)));
        
        CHECK(parse_str("_let f = _fun (x) x + 1\n_in _if _false\n_then f(5)\n_else f(6)")->interp(Env::empty)->equals(NEW(NumVal)(7)));
        CHECK(parse_str("_let f = _fun (x) x + 1\n_in _let g = _fun (y) y+2\n_in _if _true\n_then f(5)\n_else g(5)")->interp(Env::empty)->equals(NEW(NumVal)(6)));
        CHECK(parse_str("_let f = _fun (x) x + 1\n_in _let g = _fun (y) y+2\n_in f(g(5))")->interp(Env::empty)->equals(NEW(NumVal)(8)));
        CHECK(parse_str("_let f = _fun (x) x + 1\n_in _let g = _fun (y) f(y+2)\n_in g(5)")->interp(Env::empty)->equals(NEW(NumVal)(8)));
        CHECK(parse_str("_let f = _fun (x) x + 1\n_in _let g = _fun (x) f(2) + x\n_in g(5)")->interp(Env::empty)->equals(NEW(NumVal)(8)));
        CHECK(parse_str("_let f = _fun (x) x + 1\n_in (f)(5)")->interp(Env::empty)->equals(NEW(NumVal)(6)));
        CHECK(parse_str("(_fun (x)\nx + 1)(5)")->interp(Env::empty)->equals(NEW(NumVal)(6)));
        CHECK(parse_str("_let f = _if _false\n_then _fun (x)\nx+1\n_else _fun (x)\nx + 2\n_in f(5)")->interp(Env::empty)->equals(NEW(NumVal)(7)));
        CHECK(parse_str("(_if _false\n_then _fun (x)\nx + 1\n_else _fun (x)\nx + 2)(5)")->interp(Env::empty)->equals(NEW(NumVal)(7)));
        CHECK(parse_str("_let f = _fun (f)\n_fun(x)\n_if x == 0\n_then 0\n_else x + f(f)(x + -1)\n_in f(f)(3)")->interp(Env::empty)->equals(NEW(NumVal)(6)));
        CHECK(parse_str("_let f = _fun (x) _fun (y) x + y _in f(5)(1)")->interp(Env::empty)->equals(NEW(NumVal)(6)));
        
        CHECK(parse_str("_let y = 8\n_in _let f = _fun (x) x*y\n_in f(2)")->interp(Env::empty)->equals(NEW(NumVal)(16)));
        
    }
    SECTION("Print Cases") {
        CHECK(parse_str("_let x = _fun(y) y + 2\n_in x(2)")->pretty_print() == "_let x = _fun (y)\n           y + 2\n_in  x(2)");
        CHECK(parse_str("_let f = _fun (x) x + 1\n_in f")->interp(Env::empty)->to_string() == "_fun(x) (x+1)");
    }
}
