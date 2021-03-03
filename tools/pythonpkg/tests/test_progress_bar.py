import duckdb
import pandas as pd
import numpy
import datetime
import time

class TestProgressBarPandas(object):
    def test_progress(self, duckdb_cursor):
        con = duckdb.connect()
        df = pd.DataFrame({'i': numpy.arange(100000000)})

        con.register('df', df)
        con.register('df_2', df)

        con.execute("PRAGMA set_progress_bar_time=1")

        result =  con.execute("SELECT SUM(df.i) FROM df inner join df_2 on (df.i = df_2.i)").fetchall()
        con.execute("PRAGMA threads=4")
        parallel_results = con.execute("SELECT SUM(df.i) FROM df inner join df_2 on (df.i = df_2.i)").fetchall()

        assert result[0][0] == 4999999950000000
        assert parallel_results[0][0] == 4999999950000000
