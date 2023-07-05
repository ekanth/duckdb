from typing import (
    Union,
    TYPE_CHECKING
)

if TYPE_CHECKING:
	from ._typing import ColumnOrName, LiteralType, DecimalLiteral, DateTimeLiteral

from duckdb import (
    BinaryFunctionExpression,
    ConstantExpression,
    ColumnExpression,
    Expression
)

class Column:
    """
    A column in a DataFrame.

    :class:`Column` instances can be created by::

        # 1. Select a column out of a DataFrame

        df.colName
        df["colName"]

        # 2. Create from an expression
        df.colName + 1
        1 / df.colName

    .. versionadded:: 1.3.0
    """
    def __init__(self, expr: Expression):
        self.expr = expr
    
    # arithmetic operators
    def __neg__(self):
        return Column(-self.expr)

    def __add__(self, other: Column):
        return Column(self.expr + other.expr)

    def __sub__(self, other: Column):
        return Column(self.expr - other.expr)

    def __mul__(self, other: Column):
        return Column(self.expr * other.expr)

    def __div__(self, other: Column):
        return Column(self.expr * other.expr)

    def __truediv__(self, other: Column):
        return Column(self.expr / other.expr)

    def __mod__(self, other: Column):
        return Column(self.expr % other.expr)

    def __pow__(self, other: Column):
        return Column(self.expr ** other.expr)

    def __radd__(self, other: Column):
        return Column(other.expr + self.expr)

    def __rsub__(self, other: Column):
        return Column(other.expr - self.expr)

    def __rmul__(self, other: Column):
        return Column(other.expr * self.expr)

    def __rdiv__(self, other: Column):
        return Column(other.expr / self.expr)

    def __rtruediv__(self, other: Column):
        return Column(other.expr / self.expr)

    def __rmod__(self, other: Column):
        return Column(other.expr % self.expr)

    def __rpow__(self, other: Column):
        return Column(other.expr ** self.expr)

    # logistic operators
    def __eq__(  # type: ignore[override]
        self,
        other: Union["Column", "LiteralType", "DecimalLiteral", "DateTimeLiteral"],
    ) -> "Column":
        """binary function"""
        return _bin_op("equalTo")(self, other)

    def __ne__(  # type: ignore[override]
        self,
        other: Any,
    ) -> "Column":
        """binary function"""
        return _bin_op("notEqual")(self, other)

    __lt__ = _bin_op("lt")
    __le__ = _bin_op("leq")
    __ge__ = _bin_op("geq")
    __gt__ = _bin_op("gt")