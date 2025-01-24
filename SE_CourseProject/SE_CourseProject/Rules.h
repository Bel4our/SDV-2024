#pragma once
#include "GRB.h"
#define GRB_ERROR_SERIES 600
namespace GRB
{

#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

	Greibach greibach(
		NS('S'), TS('$'),
		12,
		Rule(NS('S'), GRB_ERROR_SERIES + 0,
			2,
			Rule::Chain(7, TS('d'), TS('t'), TS('f'), TS('i'), NS('O'), NS('B'), NS('S')),
			Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), TS(';'))
		),
		Rule(
			NS('B'), GRB_ERROR_SERIES + 3,
			2,
			Rule::Chain(7, TS('{'), NS('N'), TS('r'), NS('I'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(6, TS('{'), TS('r'), NS('I'), TS(';'), TS('}'), TS(';'))
		),

		Rule(NS('N'), GRB_ERROR_SERIES + 6,
			28,
			Rule::Chain(3, TS('+'), NS('K'), TS(';')),
			Rule::Chain(3, TS('/'), NS('K'), TS(';')),
			Rule::Chain(3, TS('z'), NS('K'), TS(';')),

			Rule::Chain(4, TS('+'), NS('K'), TS(';'), NS('N')),
			Rule::Chain(4, TS('/'), NS('K'), TS(';'), NS('N')),
			Rule::Chain(4, TS('z'), NS('K'), TS(';'), NS('N')),

			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),

			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),

			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),

			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),

			Rule::Chain(3, TS('w'), NS('E'), TS(';')),
			Rule::Chain(4, TS('w'), NS('E'), TS(';'), NS('N')),

			Rule::Chain(8, TS('c'), TS('('), NS('R'), TS(')'), TS('['), NS('X'), TS(']'), TS(';')),
			Rule::Chain(9, TS('c'), TS('('), NS('R'), TS(')'), TS('['), NS('X'), TS(']'), TS(';'), NS('N')),

			Rule::Chain(12, TS('c'), TS('('), NS('R'), TS(')'), TS('['), NS('X'), TS(']'), TS('!'), TS('['), NS('X'), TS(']'), TS(';')),
			Rule::Chain(13, TS('c'), TS('('), NS('R'), TS(')'), TS('['), NS('X'), TS(']'), TS('!'), TS('['), NS('X'), TS(']'), TS(';'), NS('N')),

			Rule::Chain(4, TS('i'), NS('K'), TS(';'), NS('N')),
			Rule::Chain(3, TS('i'), NS('K'), TS(';')),

			Rule::Chain(4, TS('r'), NS('I'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), NS('I'), TS(';')),

			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('R'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('R'), TS(';')),


			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('R'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('R'), TS(';'), NS('N'))
		),

		Rule(
			NS('O'), GRB_ERROR_SERIES + 1,
			2,
			Rule::Chain(2, TS('('), TS(')')),
			Rule::Chain(3, TS('('), NS('F'), TS(')'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 9,
			18,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('y')),

			Rule::Chain(2, TS('-'), TS('l')),

			Rule::Chain(3, TS('/'), NS('K'), NS('M')),
			Rule::Chain(3, TS('+'), NS('K'), NS('M')),
			Rule::Chain(3, TS('z'), NS('K'), NS('M')),

			Rule::Chain(2, TS('/'), NS('K')),
			Rule::Chain(2, TS('+'), NS('K')),
			Rule::Chain(2, TS('z'), NS('K')),


			Rule::Chain(2, TS('i'), NS('K')),
			Rule::Chain(3, TS('i'), NS('K'), NS('M')),



			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(2, TS('y'), NS('M')),

			Rule::Chain(3, TS('-'), TS('l'), NS('M')),

			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M'))

		),
		Rule(NS('M'), GRB_ERROR_SERIES + 3,
			2,
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),


		Rule(NS('F'), GRB_ERROR_SERIES + 2,
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),

		Rule(NS('W'), GRB_ERROR_SERIES + 4,
			8,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('y')),
			Rule::Chain(2, TS('-'), TS('l')),
		//	Rule::Chain(2, TS('-'), TS('i')),
			Rule::Chain(4, TS('-'), TS('l'), TS(','), NS('W')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W')),
			Rule::Chain(3, TS('y'), TS(','), NS('W'))

		),

		Rule(NS('R'), GRB_ERROR_SERIES + 7,
			12,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('y')),
			Rule::Chain(3, TS('i'), TS('s'), TS('i')),
			Rule::Chain(3, TS('i'), TS('s'), TS('l')),
			Rule::Chain(3, TS('l'), TS('s'), TS('i')),
			Rule::Chain(3, TS('l'), TS('s'), TS('l')),
			Rule::Chain(3, TS('y'), TS('s'), TS('y')),
			Rule::Chain(3, TS('l'), TS('s'), TS('y')),
			Rule::Chain(3, TS('y'), TS('s'), TS('l')),
			Rule::Chain(3, TS('i'), TS('s'), TS('y')),
			Rule::Chain(3, TS('y'), TS('s'), TS('i'))
		),
		Rule(
			NS('K'), GRB_ERROR_SERIES + 8,
			2,
			Rule::Chain(3, TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),
		Rule(
			NS('X'), GRB_ERROR_SERIES + 11,
			24,
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('X')),
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),

			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('X')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),

			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('R'), TS(';'), NS('X')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('R'), TS(';')),

			Rule::Chain(5, TS('i'), TS('='), NS('R'), TS(';'), NS('X')),
			Rule::Chain(4, TS('i'), TS('='), NS('R'), TS(';')),

			Rule::Chain(4, TS('r'), NS('I'), TS(';'), NS('X')),
			Rule::Chain(3, TS('r'), NS('I'), TS(';')),

			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('X')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),

			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('X')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),

			Rule::Chain(4, TS('w'), NS('E'), TS(';'), NS('X')),
			Rule::Chain(3, TS('w'), NS('E'), TS(';')),

			Rule::Chain(4, TS('+'), NS('K'), TS(';'), NS('X')),
			Rule::Chain(3, TS('+'), NS('K'), TS(';')),

			Rule::Chain(4, TS('z'), NS('K'), TS(';'), NS('X')),
			Rule::Chain(3, TS('z'), NS('K'), TS(';')),

			Rule::Chain(4, TS('/'), NS('K'), TS(';'), NS('X')),
			Rule::Chain(3, TS('/'), NS('K'), TS(';')),

			Rule::Chain(4, TS('i'), NS('K'), TS(';'), NS('X')),
			Rule::Chain(3, TS('i'), NS('K'), TS(';'))

		),
		Rule(
			NS('I'), GRB_ERROR_SERIES + 4,
			3,
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('y'))

		)
	);

}