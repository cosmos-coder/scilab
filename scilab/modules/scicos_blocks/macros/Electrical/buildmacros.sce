//  Scicos
//
//  Copyright (C) INRIA - Allan Cornet <allan.cornet@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//
//------------------------------------
if (isdef("genlib") == %f) then
    exec(SCI+"/modules/functions/scripts/buildmacros/loadgenlib.sce");
end
//------------------------------------
genlib("Electricallib","SCI/modules/scicos_blocks/macros/Electrical",%f,%t);
//------------------------------------
if with_modelica_compiler() then

    // create models file in current directory
    models = findfiles(pwd(),"*.mo");
    // Exception, we don't build 'Maths.mo' :(
    // TO DO : modify this (Simoné or Laurent) move files
    pos = find(models == "Maths.mo");
    if(isempty(pos) == %f) then
        models = [models(1:pos-1) ; models(pos+1:size(models,"*"))];

        fd = mopen("models","wt");
        for i=1:size(models,"*")
            mputstr(models(i) + ascii(13),fd);
        end
        mclose(fd);
    end
    // generate moc files
    //exec("../../src/scripts/genmoc.sce");
end;
//------------------------------------
