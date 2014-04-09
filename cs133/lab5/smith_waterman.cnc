[ char alignment_score_matrix ];
[ char* string_1] ;
[ char* string_2 ];
[ int** score ];
[ char* file_name_1 ];
[ char* file_name_2 ];
< int [1] Argu_Error >;
< int [1] Read_Error >;
< int [1] DoneScore>;


< Argu_Error > :: ( read_file1 );
< Argu_Error > :: ( read_file2 );

[ file_name_1: i ] -> ( read_file1: i );
( read_file1: i ) -> [ string_1: i ], <Read_Error: i >;

[ file_name_2: i ] -> ( read_file2: i );
( read_file2: i ) -> [ string_2: i ] , <Read_Error: i >;

< Read_Error > :: ( score_gen );
( score_gen: length_file1 length_file2 ) -> [score: j], < DoneScore: j>;

< DoneScore > :: (smith_waterman );
[ string_1: k] -> (smith_waterman: k );
[ string_2: k] -> (smith_waterman: k );
[score: k] -> (smith_waterman: k);
[alignment_score_matrix: k] -> (smith_waterman: k);
(smith_waterman: k) -> [score: k];

env -> [file_name_1], [file_name_2];
env -> <Argu_Error: d>;
[score] -> env;







