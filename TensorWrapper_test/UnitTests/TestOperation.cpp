#include <TensorWrapper/Operation.hpp>
#include <TensorWrapper/OperationImpls.hpp>
#include <TensorWrapper/TensorPtr.hpp>
#include <Eigen/Dense>

#include "TestHelpers.hpp"
#include <vector>

/** \file Tests to ensure the Operation class as well as the various
 *  OperationImpl classes are working correctly.
 *
 *
 */
using namespace TWrapper;
using namespace detail_;

using pTensor=TensorPtr<2,double>;

int main()
{
    Tester tester("Testing Operation class");

    constexpr TensorTypes type=TensorTypes::EigenMatrix;
    Eigen::MatrixXd value=Eigen::MatrixXd::Zero(10,10);
    pTensor my_tensor(type,value);
    const pTensor& const_mytensor=const_cast<const pTensor&>(my_tensor);
    auto& actual_tensor=my_tensor.cast<type>();

    //Dereference Op
    using DeRef_t=DeRef<2,double>;
    auto deref_op=make_op<2,double,DeRef_t>(my_tensor);
    auto& deref_result=deref_op.template eval<type>();
    tester.test("Deref result",deref_result==value);
    tester.test("Not a copy",&deref_result==&actual_tensor);

    auto cderef_op=make_op<2,double,DeRef_t>(const_mytensor);
    auto& cderef_op_result=cderef_op.template eval<type>();
    tester.test("Const Deref result",cderef_op_result==value);
    tester.test("Const Not a copy",&cderef_op_result==&actual_tensor);

    //Dimensions Op
    using DimOp=DimsOp<2,double>;
    Shape<2> right({10,10},false);
    auto dim_op=make_op<2,double,DimOp>(value);
    auto dim_op_result=dim_op.template eval<type>();
    tester.test("Dimensions Op",dim_op_result==right);

    //Add Op
    using Add=AddOp<2,double>;
    auto add_op=make_op<2,double,Add>(value,value);
    auto add_result=add_op.template eval<type>();
    tester.test("Addition",add_result==(value+value));

    //Equal
    using Equal=EqualOp<2,double>;
    auto equal_op=make_op<2,double,Equal>(value,value);
    auto equal_result=equal_op.template eval<type>();
    tester.test("Equal",equal_result=(value==value));

    return tester.results();
}
