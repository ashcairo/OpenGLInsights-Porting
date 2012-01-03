/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCBaseTypes.js
 * Description : Contains base structures.
 *
 * Created     : 30/11/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCList()
{
	this.list = new Array();
	this.length = 0;
}


CCList.prototype.clear = function()
{
	var list = this.list;
    list.length = 0;
	this.length = 0;
}									


CCList.prototype.add = function(object)
{
	this.list[this.length++] = object;
}


CCList.prototype.remove = function(object)
{
	var list = this.list;
	var length = this.length;
	for( var i=0; i<length; ++i )
	{
		if( list[i] == object )
		{
			this.length--;
			length = this.length;
			for( var j=i; j<length; ++j )
			{
				list[j] = list[j+1];
			}
			return true;
		}
	}
    
	return false;
}


// Re-insert from the back of the queue
CCList.prototype.reinsert = function(object, index)
{
//    ASSERT( list[length-1] == object );
    var list = this.list;
    var length = this.length;
    for( var i=length-1; i>index; --i )
    {
        list[i] = list[i-1];
    }
    list[index] = object;
}


CCList.prototype.find = function(object)
{
	var list = this.list;
	var length = this.length;
	for( var i=0; i<length; ++i )
	{
		if( list[i] == object )
		{
			return true;
		}
	}
	return false;
}


CCList.prototype.last = function()
{
    return this.list[this.length-1];
}


/* TableComponent	 												*/
/* Base HTML table component which can be processed by the engine.	*/
function TableComponent(parent, selectable)
{
	this.callbacks = new Array();
	
	var table = this.table = document.createElement( 'table' );
	table.cellPadding = 0;
	table.cellSpacing = 0;
		
	var style = this.style = table.style;
	style.marginLeft = 0;
	style.marginRight = 0;
	style.marginTop = 0;
	style.marginBottom = 0;
    
    if( selectable == false )
    {
        style.cursor = 'pointer';
        table.unselectable = true;
        style.MozUserSelect = 'none';
    }
	
	this.dimensions = new ObjectDimensions( this.table );
	
	style.position = 'absolute';
	style.left = '0px';
	style.top = '0px';
	
	var tBody = this.tBody = document.createElement( 'tbody' );
	table.appendChild( tBody );
	
	this.tr = new Array();
	this.addRow();
	this.addColumn();
	this.td = this.tr[0].td[0];

    if( parent )
    {
        parent.appendChild( this.table );
    }
}


TableComponent.prototype.addRow = function()
{
	var index = this.tr ? this.tr.length : 0;
	var tr = this.tr[index] = document.createElement( 'tr' );
	tr.td = new Array();
	this.tBody.appendChild( tr );
	return tr;
}


TableComponent.prototype.addColumnToRow = function(row)
{
	if( row >= this.tr.length )
	{
		alert( 'TableComponent.prototype.addColumn() - invalid row' );
		return;
	}
	
	var index = this.tr[row].td ? this.tr[row].td.length : 0;
	var td = this.tr[row].td[index] = document.createElement( 'td' );		
	this.tr[row].appendChild( td );
	return td;
}


TableComponent.prototype.addColumn = function()
{
	return this.addColumnToRow( this.tr.length-1 );
}