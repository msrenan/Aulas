
Reuso dinâmico de registros:

Registro: id, ok, \[campos]

se ok 0 -> um dos campos passa a apontar para o próximo registro deletado, ou NULL.

o id 0 é um registro de controle, e um dos campos passa a apontar para o primeiro registro deletado ou NULL.

Se precisar adiiconar um registro novo, sobrescrevemos o primeiro deletado.

 - Vantagens
	 - Aproveitamento de espaço
	 - Eficiência de tempo pra achar onde inserir
	 - 