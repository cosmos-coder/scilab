// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//

loadXcosLibs();

// Diagram
scs_m = scicos_diagram();

fields = getfield(1, scs_m);
fieldsRef = ["diagram" "props" "objs" "version" "contrib"];
assert_checkequal(fields, fieldsRef);

props = getfield(2, scs_m);
assert_checkequal(props, scs_m.props);

objs = getfield(3, scs_m);
assert_checkequal(objs, scs_m.objs);

version = getfield(4, scs_m);
assert_checkequal(version, scs_m.version);

contrib = getfield(5, scs_m);
assert_checkequal(contrib, scs_m.contrib);

refMsg = msprintf(_("%s: Wrong value for input argument #%d: At most %d expected.\n"), "getfield", 1, 5);
assert_checkerror("getfield(6, scs_m)", refMsg);


// Block
b = scicos_block();

fields = getfield(1, b);
fieldsRef = ["Block" "graphics" "model" "gui" "doc"];
assert_checkequal(fields, fieldsRef);

props = getfield(2, b);
assert_checkequal(props, b.graphics);

objs = getfield(3, b);
assert_checkequal(objs, b.model);

version = getfield(4, b);
assert_checkequal(version, b.gui);

contrib = getfield(5, b);
assert_checkequal(contrib, b.doc);

refMsg = msprintf(_("%s: Wrong value for input argument #%d: At most %d expected.\n"), "getfield", 1, 5);
assert_checkerror("getfield(6, b)", refMsg);