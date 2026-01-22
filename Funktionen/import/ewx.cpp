#include "ewx.h"

werkstueck import_ewx(QString dateiinhalt)
{
    werkstueck w;

    text_zw tz;
    tz.set_text(dateiinhalt,'\n');
    //WST-Länge ermitteln:
    for(uint i=0; i<tz.count() ;i++)
    {
        QString parname = "'Maß, Boxlänge l1':";
        if(tz.at(i).contains(parname))
        {
            QString parwert = text_rechts(tz.at(i),parname);
            parwert = text_mitte(parwert, " '", "',");
            w.set_laenge(parwert);
            break; //for
        }
    }
    //WST-Breite ermitteln:
    for(uint i=0; i<tz.count() ;i++)
    {
        QString parname = "'Maß, Boxlänge l2':";
        if(tz.at(i).contains(parname))
        {
            QString parwert = text_rechts(tz.at(i),parname);
            parwert = text_mitte(parwert, " '", "',");
            w.set_breite(parwert);
            break; //for
        }
    }
    //WST-Breite ermitteln:
    for(uint i=0; i<tz.count() ;i++)
    {
        QString parname = "'Maß, Boxlänge l3':";
        if(tz.at(i).contains(parname))
        {
            QString parwert = text_rechts(tz.at(i),parname);
            parwert = text_mitte(parwert, " '", "',");
            w.set_dicke(parwert);
            break; //for
        }
    }

    //Bearbeitungen erfassen:
    ewx_reference ref;
    ref.set_wst_dicke(w.dicke());
    text_zw fkon;
    fraeseraufruf fauf;
    fauf.set_radkor(FRKOR_M); //Default-Wert
    bool wstkontur = false;
    for(uint i=0; i<tz.count() ;i++)
    {
        QString zeile = tz.at(i);
        if(zeile.contains(" = Reference("))
        {
            ref.set_text(zeile);
        }
        if(zeile.contains("path1 = ["))
        {
            wstkontur = true;
            ref.set_text("reference2 = Reference(pt=(0.0000, 0.0000, 0.0000), vz=(0.0000, 0.0000, 1.0000), vx=(1.0000, 0.0000, 0.0000))");
        }
        if(wstkontur == true  &&  zeile.contains("]")) //Außenkontur WST
        {
            fauf.set_bezug(ref.bezug());
            if(fkon.at(0).contains(BEARBART_FRAESERGERADE))
            {
                fraesergerade fg;
                fg.set_text(fkon.at(0));
                fauf.set_pos(fg.sp());
            }else if(fkon.at(0).contains(BEARBART_FRAESERBOGEN))
            {
                fraeserbogen fb;
                fb.set_text(fkon.at(0));
                fauf.set_pos(fb.bog_ptr()->spu());
            }
            w.neue_bearbeitung(fauf.text());
            for(uint ii=0 ; ii<fkon.count() ; ii++)
            {
                w.neue_bearbeitung(fkon.at(ii));
            }
            fkon.clear();
            fauf.set_radkor(FRKOR_M); //Default-Wert
            wstkontur = false;
        }
        if(zeile.contains("name='PYTHA_ROUTE'")  ||  zeile.contains("name='PYTHA_POCKET'")) //Fräsbahn oder Tasche
        {
            /*
            Die PYTHA_POCKET muss keine rta oder kta sein, sondern kann auch eine Freiformtasche sein

            path2 = [
                Line(pt1=(444.0000, 32.0000, 0.), pt2=(83.0000, 32.0000, 0.), thickness=16.0000),
                Arc(pt1=(83.0000, 32.0000, 0.), ptc=(83.0000, 57.0000, 0.), pt2=(58.0000, 57.0000, 0.), cw=True, thickness=16.0000),
                Line(pt1=(58.0000, 57.0000, 0.), pt2=(58.0000, 418.0000, 0.), thickness=16.0000),
                Arc(pt1=(58.0000, 418.0000, 0.), ptc=(83.0000, 418.0000, 0.), pt2=(83.0000, 443.0000, 0.), cw=True, thickness=16.0000),
                Line(pt1=(83.0000, 443.0000, 0.), pt2=(444.0000, 443.0000, 0.), thickness=16.0000),
                Arc(pt1=(444.0000, 443.0000, 0.), ptc=(444.0000, 418.0000, 0.), pt2=(469.0000, 418.0000, 0.), cw=True, thickness=16.0000),
                Line(pt1=(469.0000, 418.0000, 0.), pt2=(469.0000, 57.0000, 0.), thickness=16.0000),
                Arc(pt1=(469.0000, 57.0000, 0.), ptc=(444.0000, 57.0000, 0.), pt2=(444.0000, 32.0000, 0.), cw=True, thickness=16.0000),
                ]
            notes3 = {
            'depth': 16.0000,
            'tool_name': 'InnenKontur_W12_D8_NL23',
            'tool': 12,
            }
            part1.add_layer(geometries=path2, reference=reference2, name='PYTHA_POCKET', notes=notes3)
            */
            bool rta_gefunden = false;
            if(fkon.count() == 5)
            {
                /*
                path2 = [
                    Line(pt1=(344.6575, 132.7342, 0.), pt2=(444.6575, 132.7342, 0.), thickness=4.0000),
                    Line(pt1=(444.6575, 132.7342, 0.), pt2=(444.6575, 282.7342, 0.), thickness=4.0000),
                    Line(pt1=(444.6575, 282.7342, 0.), pt2=(244.6575, 282.7342, 0.), thickness=4.0000),
                    Line(pt1=(244.6575, 282.7342, 0.), pt2=(244.6575, 132.7342, 0.), thickness=4.0000),
                    Line(pt1=(244.6575, 132.7342, 0.), pt2=(344.6575, 132.7342, 0.), thickness=4.0000),
                    ]
                */
                rta_gefunden = true;
                fraesergerade fg1, fg2, fg3, fg4, fg5;
                for(uint ii=0 ; ii<fkon.count() ; ii++)
                {
                    QString zeile = fkon.at(ii);
                    if(zeile.contains(BEARBART_FRAESERGERADE))
                    {
                        if(ii==0)
                        {
                            fg1.set_text(zeile);
                        }else if(ii==1)
                        {
                            fg2.set_text(zeile);
                        }else if(ii==2)
                        {
                            fg3.set_text(zeile);
                        }else if(ii==3)
                        {
                            fg4.set_text(zeile);
                        }else if(ii==4)
                        {
                            fg5.set_text(zeile);
                        }

                    }else
                    {
                        rta_gefunden = false;
                        break;//for
                    }
                }
                if(rta_gefunden == true)
                {
                    //Annahme: fg1 und fg5 zusammen sind eine Seite der rta
                    strecke s1_5 = fg1.strecke_();
                    s1_5.set_stapu(fg5.strecke_().stapu());
                    strecke s2 = fg2.strecke_();
                    strecke s3 = fg3.strecke_();
                    strecke s4 =fg4.strecke_();
                    if(s1_5.laenge2d() == s3.laenge2d() &&\
                                                               s2.laenge2d() == s4.laenge2d()   )
                    {
                        rta_gefunden = true;
                    }else
                    {
                        rta_gefunden = false;
                    }
                    double ti;
                    if(fg1.tiSta() == fg1.tiEnd() &&\
                                                         fg1.tiSta() == fg2.tiSta() &&\
                              fg1.tiSta() == fg2.tiEnd() &&\
                              fg1.tiSta() == fg3.tiSta() &&\
                              fg1.tiSta() == fg3.tiEnd() &&\
                              fg1.tiSta() == fg4.tiSta() &&\
                              fg1.tiSta() == fg4.tiEnd() &&\
                              fg1.tiSta() == fg5.tiSta() &&\
                              fg1.tiSta() == fg5.tiEnd() )
                    {
                        ti = fg1.tiSta();
                    }else
                    {
                        rta_gefunden = false;
                    }
                    if(rta_gefunden == true)
                    {
                        rechtecktasche rta;
                        rta.set_wkznum(fauf.wkznum());
                        rta.set_bezug(ref.bezug());
                        rta.set_tiefe(ti);
                        strecke s1_5__3;
                        s1_5__3.set_stapu(fg1.strecke_().stapu());
                        s1_5__3.set_endpu(s3.mipu());
                        rta.set_mipu(s1_5__3.mipu());
                        rta.set_drewi(s1_5__3.wink());
                        rta.set_laenge(s2.laenge2d());
                        rta.set_breite(s3.laenge2d());
                        w.neue_bearbeitung(rta.text());
                    }
                }
            }
            //Auch wenn Tasche erkannt wird soll zusätzlich fkon eingelesen werden:
            fauf.set_bezug(ref.bezug());
            fauf.set_bezug(ref.bezug());
            if(fkon.at(0).contains(BEARBART_FRAESERGERADE))
            {
                fraesergerade fg;
                fg.set_text(fkon.at(0));
                fauf.set_pos(fg.sp());
            }else if(fkon.at(0).contains(BEARBART_FRAESERBOGEN))
            {
                fraeserbogen fb;
                fb.set_text(fkon.at(0));
                fauf.set_pos(fb.bog_ptr()->spu());
            }
            w.neue_bearbeitung(fauf.text());
            for(uint ii=0 ; ii<fkon.count() ; ii++)
            {
                w.neue_bearbeitung(fkon.at(ii));
            }

            fkon.clear();
            fauf.set_radkor(FRKOR_M); //Default-Wert
        }
        if(zeile.contains("name='PYTHA_GROOVE'")) //Nut
        {
            /*
            path2 = [
                Line(pt1=(-5.0000, 396.0000, 0.), pt2=(705.0000, 396.0000, 0.), thickness=6.5000),
                Line(pt1=(705.0000, 396.0000, 0.), pt2=(705.0000, 404.5000, 0.), thickness=6.5000),
                Line(pt1=(705.0000, 404.5000, 0.), pt2=(-5.0000, 404.5000, 0.), thickness=6.5000),
                Line(pt1=(-5.0000, 404.5000, 0.), pt2=(-5.0000, 396.0000, 0.), thickness=6.5000),
                ]
            notes3 = {
                'depth': 6.5000,
                'tool_name': '11',
                'tool': 11,
                'extensionMode': 2,
                'routeHint': 0,
                }
            part1.add_layer(geometries=path2, reference=reference2, name='PYTHA_GROOVE', notes=notes3)
            */
            bool nut_gefunden = false;
            if(fkon.count() == 4)
            {
                nut_gefunden = true;
                fraesergerade fg1, fg2, fg3, fg4;
                for(uint ii=0 ; ii<fkon.count() ; ii++)
                {
                    QString zeile = fkon.at(ii);
                    if(zeile.contains(BEARBART_FRAESERGERADE))
                    {
                        if(ii==0)
                        {
                            fg1.set_text(zeile);
                        }else if(ii==1)
                        {
                            fg2.set_text(zeile);
                        }else if(ii==2)
                        {
                            fg3.set_text(zeile);
                        }else if(ii==3)
                        {
                            fg4.set_text(zeile);
                        }
                    }else
                    {
                        nut_gefunden = false;
                        break;//for
                    }
                }
                if(nut_gefunden == true)
                {
                    strecke s1, s2;//Hirnenden der Nut
                    if(fg1.strecke_().laenge2d() < fg2.strecke_().laenge2d())
                    {
                        s1 = fg1.strecke_();
                        if(s1.laenge2d() == fg3.strecke_().laenge2d())
                        {
                            s2 = fg3.strecke_();
                        }else if(s1.laenge2d() == fg4.strecke_().laenge2d())
                        {
                            s2 = fg4.strecke_();
                        }else
                        {
                            nut_gefunden = false;
                        }
                    }else if(fg1.strecke_().laenge2d() > fg2.strecke_().laenge2d())
                    {
                        s1 = fg2.strecke_();
                        if(s1.laenge2d() == fg3.strecke_().laenge2d())
                        {
                        }else if(s1.laenge2d() == fg4.strecke_().laenge2d())
                        {
                            s2 = fg4.strecke_();
                        }else
                        {
                            nut_gefunden = false;
                        }
                    }else if(fg1.strecke_().laenge2d() == fg2.strecke_().laenge2d())
                    {
                        if(fg3.strecke_().laenge2d() == fg4.strecke_().laenge2d())
                        {
                            if(fg1.strecke_().laenge2d() < fg3.strecke_().laenge2d())
                            {
                                s1 = fg1.strecke_();
                                s2 = fg2.strecke_();
                            }else
                            {
                                s1 = fg3.strecke_();
                                s2 = fg4.strecke_();
                            }
                        }else
                        {
                            nut_gefunden = false;
                        }
                    }else
                    {
                        nut_gefunden = false;
                    }
                    double ti;
                    if(fg1.tiSta() == fg1.tiEnd() &&\
                                                         fg1.tiSta() == fg2.tiSta() &&\
                              fg1.tiSta() == fg2.tiEnd() &&\
                              fg1.tiSta() == fg3.tiSta() &&\
                              fg1.tiSta() == fg3.tiEnd() &&\
                              fg1.tiSta() == fg4.tiSta() &&\
                              fg1.tiSta() == fg4.tiEnd() )
                    {
                        ti = fg1.tiSta();
                    }else
                    {
                        nut_gefunden = false;
                    }
                    if(nut_gefunden == true)
                    {
                        nut nu;
                        nu.set_stapu(s1.mipu());
                        nu.set_endpu(s2.mipu());
                        nu.set_breite(s1.laenge2d());
                        nu.set_bezug(ref.bezug());
                        nu.set_tiefe(ti);
                        w.neue_bearbeitung(nu.text());
                    }
                }
            }
            if(nut_gefunden == false)
            {
                fauf.set_bezug(ref.bezug());
                fauf.set_bezug(ref.bezug());
                if(fkon.at(0).contains(BEARBART_FRAESERGERADE))
                {
                    fraesergerade fg;
                    fg.set_text(fkon.at(0));
                    fauf.set_pos(fg.sp());
                }else if(fkon.at(0).contains(BEARBART_FRAESERBOGEN))
                {
                    fraeserbogen fb;
                    fb.set_text(fkon.at(0));
                    fauf.set_pos(fb.bog_ptr()->spu());
                }
                w.neue_bearbeitung(fauf.text());
                for(uint ii=0 ; ii<fkon.count() ; ii++)
                {
                    w.neue_bearbeitung(fkon.at(ii));
                }
            }
            fkon.clear();
            fauf.set_radkor(FRKOR_M); //Default-Wert
        }

        if(zeile.contains("'tool_name':"))
        {
            QString name = text_rechts(zeile, "'tool_name':");
            name = text_mitte(name, " '", "',");
            fauf.set_wkznum(name);
        }else if(zeile.contains("'compensation':"))
        {
            if(zeile.contains("'right'"))
            {
                fauf.set_radkor(FRKOR_R);
            }else if(zeile.contains("'left'"))
            {
                fauf.set_radkor(FRKOR_L);
            }else
            {
                fauf.set_radkor(FRKOR_M);
            }
        }else if(zeile.contains("'depth':"))
        {
            QString ti = text_rechts(zeile, "'depth':");
            ti = text_mitte(ti, " ", ",");
            fauf.set_tiefe(ti);
        }

        if(zeile.contains("Hole("))
        {
            punkt3d ewxpos;
            text_zw parameter;
            parameter.set_text(selektiereEintrag(zeile, "pos=(", ")"), ',');
            ewxpos.set_x(parameter.at(0));
            ewxpos.set_y(parameter.at(1));
            ewxpos.set_z(parameter.at(2));
            bohrung bo;
            bo.set_dm(text_mitte(zeile, "diameter=", ","));
            bo.set_tiefe(text_mitte(zeile, "depth=", ","));
            bo.set_bezug(ref.bezug());
            bo.set_mipu(ref.bearbpos(ewxpos));
            w.neue_bearbeitung(bo.text());
        }else if(zeile.contains("Line("))
        {
            text_zw parameter;
            fraesergerade fg;
            punkt3d stapu;
            parameter.set_text(selektiereEintrag(zeile, "pt1=(", ")"), ',');
            stapu.set_x(parameter.at(0));
            stapu.set_y(parameter.at(1));
            stapu.set_z(parameter.at(2));
            stapu = ref.bearbpos(stapu);
            fg.set_startpunkt(stapu);
            punkt3d endpu;
            parameter.set_text(selektiereEintrag(zeile, "pt2=(", ")"), ',');
            endpu.set_x(parameter.at(0));
            endpu.set_y(parameter.at(1));
            endpu.set_z(parameter.at(2));
            endpu = ref.bearbpos(endpu);
            fg.set_endpunkt(endpu);
            QString ti = selektiereEintrag(zeile, "thickness=", "),");
            fg.set_tiSta(ti);
            fg.set_tiEnd(ti);
            fg.set_bezug(ref.bezug());
            fkon.add_hi(fg.text());
        }else if(zeile.contains("Arc("))
        {
            text_zw parameter;
            punkt3d stapu;
            parameter.set_text(selektiereEintrag(zeile, "pt1=(", ")"), ',');
            stapu.set_x(parameter.at(0));
            stapu.set_y(parameter.at(1));
            stapu.set_z(parameter.at(2));
            stapu = ref.bearbpos(stapu);
            punkt3d endpu;
            parameter.set_text(selektiereEintrag(zeile, "pt2=(", ")"), ',');
            endpu.set_x(parameter.at(0));
            endpu.set_y(parameter.at(1));
            endpu.set_z(parameter.at(2));
            endpu = ref.bearbpos(endpu);
            punkt3d mipu;
            parameter.set_text(selektiereEintrag(zeile, "ptc=(", ")"), ',');
            mipu.set_x(parameter.at(0));
            mipu.set_y(parameter.at(1));
            mipu.set_z(parameter.at(2));
            mipu = ref.bearbpos(mipu);
            strecke s;
            s.set_stapu(stapu);
            s.set_endpu(mipu);
            double rad = s.laenge2d();
            QString ti = selektiereEintrag(zeile, "thickness=", "),");
            QString uzs_qstring = selektiereEintrag(zeile, ", cw=", ", ");
            bool uzs;
            if(uzs_qstring == "False")
            {
                uzs = false;
            }else
            {
                uzs = true;
            }
            if(ref.bezug() == WST_BEZUG_UNSEI)
            {
                uzs = !uzs;
            }            
            bogen b;
            b.set_uzs(uzs);
            b.set_bogen(stapu, endpu, rad, uzs);
            fraeserbogen fb;
            fb.set_bogen(b);
            fb.set_tiSta(ti.toDouble());
            fb.set_tiEnd(ti.toDouble());

            fkon.add_hi(fb.text());
        }
    }

    return w;
}
