
/**
 *	Archivo pml que modela y verifica los ejercicios 12, 13 y 14 de ISEL2020.
 *
 *
 *
 *
 *	Entradas:
 *		btn_led : boton para encender el led
 *		btn_alarma : boton para encender la alarma
 *		T_led : Temporizador del led
 *		T_alarma : Temporizador de la alarma
 *
 *  Variables:
 *    estado_alarma : estado actual de la alarma
 *    estado_led : estado actual de la alarma
 *    d1,d2,d3 : digitos de la alarma
 *
 *	Salidas:
 *	 alarma
 *	 led
**/

ltl led_on {
  [](btn_led -> <>(led && T_led);
}

ltl led_time {
  [](led -> (led U T_led));
}

ltl alarma1 {
  []((estado_alarma==idle) && btn_alarma->);
}
