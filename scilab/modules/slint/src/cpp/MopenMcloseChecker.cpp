/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>

#include "checkers/MopenMcloseChecker.hxx"

namespace slint
{
void MopenMcloseChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        fd.push(std::map<symbol::Symbol, Location>());
    }
    else if (e.isCallExp())
    {
        const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
        if (ce.getName().isSimpleVar())
        {
            const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
            if (name == L"mopen")
            {
                if (ce.getParent()->isAssignExp())
                {
                    const ast::AssignExp & ae = *static_cast<const ast::AssignExp *>(e.getParent());
                    if (ae.getLeftExp().isSimpleVar())
                    {
                        const symbol::Symbol & Lsym = static_cast<const ast::SimpleVar &>(ae.getLeftExp()).getSymbol();
                        fd.top().emplace(Lsym, ae.getLeftExp().getLocation());
                    }
                    else if (ae.getLeftExp().isArrayListExp())
                    {
                        const ast::exps_t & exps = ae.getLeftExp().getExps();
                        if (!exps.empty())
                        {
                            const ast::Exp & first = *exps.front();
                            if (first.isSimpleVar())
                            {
                                const symbol::Symbol & Lsym = static_cast<const ast::SimpleVar &>(first).getSymbol();
                                fd.top().emplace(Lsym, first.getLocation());
                            }
                        }
                    }
                }
                else
                {
                    result.report(context, e.getLocation(), *this, _("Open file is not assigned."));
                }
            }
            else if (name == L"mclose")
            {
                const ast::exps_t args = ce.getArgs();
                if (!args.empty())
                {
                    const ast::Exp & first = *args.front();
                    if (first.isStringExp() && static_cast<const ast::StringExp &>(first).getValue() == L"all")
                    {
                        result.report(context, e.getLocation(), *this, _("The instruction mclose(\"all\") may have any side effets."));
                        fd.top().clear();
                    }
                    else if (first.isSimpleVar())
                    {
                        const symbol::Symbol & Lsym = static_cast<const ast::SimpleVar &>(first).getSymbol();
                        fd.top().erase(Lsym);
                    }
                }
            }
        }
    }
}

void MopenMcloseChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        if (!fd.top().empty())
        {
            std::wostringstream wos;
            auto & map = fd.top();
            for (auto i = map.begin(), end = std::prev(map.end()); i != end; ++i)
            {
                wos << i->first.getName() << L", ";
            }
            wos << std::prev(map.end())->first.getName();
            result.report(context, e.getLocation(), *this, _("Open files not closed: %s."), wos.str());
        }
        fd.pop();
    }
}

const std::string MopenMcloseChecker::getName() const
{
    return "MopenMcloseChecker";
}
}